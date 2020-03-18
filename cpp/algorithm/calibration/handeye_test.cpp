#include <iostream>
#include <gtest/gtest.h>
#include <random>

#include "handeye/navy_calibration.h"
#include "handeye/tsai_calibration.h"

#include <Eigen/Core>
#include <Eigen/Geometry>

typedef Eigen::Matrix4d Pose;
typedef Eigen::Matrix4d Transform;
typedef std::vector<Pose, Eigen::aligned_allocator<Pose>> PoseVec;
typedef std::vector<Transform, Eigen::aligned_allocator<Transform>> TransVec;
static std::default_random_engine e;

Pose fromRPYXYZ(double roll, double pitch, double yaw, double x, double y, double z)
{
    Pose pose = Pose::Identity();
    Eigen::Quaterniond quaterniond = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
                                     Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY()) *
                                     Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX());
    Eigen::Vector3d trs;
    trs << x, y, z;
    pose.block(0, 0, 3, 3) = quaterniond.toRotationMatrix();
    pose.block(0, 3, 3, 1) = trs;
    return pose;
}

void toRPYXYZ(const Pose &pose, double &roll, double &pitch, double &yaw, double &x, double &y, double &z)
{
    Eigen::Matrix3d rot = pose.block(0, 0, 3, 3);
    Eigen::Vector3d euler = rot.eulerAngles(2, 1, 0);
    yaw = euler(0);
    pitch = euler(1);
    roll = euler(2);
    x = pose(0, 3);
    y = pose(1, 3);
    z = pose(2, 3);
}

void initRandomMotionWithoutNoise(TransVec &A, TransVec &B, const Transform &transB2A)
{
    e.seed(std::time(nullptr));
    std::normal_distribution<double> motionAngleDistribution(0, M_PI * 0.35);
    std::normal_distribution<double> motionPosDistribution(0, 4.0);

    double roll = M_PI * 0.3, pitch = M_PI * 0.1, yaw = M_PI * 0.8;
    double x = 0.3, y = 0.1, z = 0.8;

    Pose preMotionPoseA = fromRPYXYZ(roll, pitch, yaw, x, y, z);
    Pose preMotionPoseB = preMotionPoseA * transB2A;

    for (int i = 0; i < 10; i++)
    {
        roll += motionAngleDistribution(e);
        pitch += motionAngleDistribution(e);
        yaw += motionAngleDistribution(e);

        x += motionPosDistribution(e);
        y += motionPosDistribution(e);
        z += motionPosDistribution(e);

        Pose curMotionPoseA = fromRPYXYZ(roll, pitch, yaw, x, y, z);
        Pose curMotionPoseB = curMotionPoseA * transB2A;

        A.push_back(preMotionPoseA.inverse() * curMotionPoseA);
        B.push_back(preMotionPoseB.inverse() * curMotionPoseB);

        preMotionPoseA = curMotionPoseA;
        preMotionPoseB = curMotionPoseB;
    }
}

void initRandomMotionWithNoise(TransVec &A, TransVec &B, const Transform &transB2A)
{
    e.seed(std::time(nullptr));
    std::normal_distribution<double> motionAngleDistribution(0, M_PI * 0.35);
    std::normal_distribution<double> motionPosDistribution(0, 4.0);
    std::normal_distribution<double> noiseAngleDistribution(0, M_PI * 0.01);
    std::normal_distribution<double> noisePosDistribution(0, 0.01);

    double motionRollA = M_PI * 0.3, motionPitchA = M_PI * 0.1, motionYawA = M_PI * 0.8;
    double motionXA = 0.3, motionYA = 0.1, motionZA = 0.8;

    Pose preMotionPoseA = fromRPYXYZ(motionRollA, motionPitchA, motionYawA, motionXA, motionYA, motionZA);
    Pose preMotionPoseB = preMotionPoseA * transB2A;

    Pose preMeasurePoseA = preMotionPoseA;
    Pose preMeasurePoseB = preMotionPoseB;

    for (int i = 0; i < 10; i++)
    {
        motionRollA += motionAngleDistribution(e);
        motionPitchA += motionAngleDistribution(e);
        motionYawA += motionAngleDistribution(e);
        motionXA += motionPosDistribution(e);
        motionYA += motionPosDistribution(e);
        motionZA += motionPosDistribution(e);

        Pose curMotionPoseA = fromRPYXYZ(motionRollA, motionPitchA, motionYawA, motionXA, motionYA, motionZA);

        double measureRollA = motionRollA + noiseAngleDistribution(e);
        double measurePitchA = motionPitchA + noiseAngleDistribution(e);
        double measureYawA = motionYawA + noiseAngleDistribution(e);
        double measureXA = motionXA + noisePosDistribution(e);
        double measureYA = motionYA + noisePosDistribution(e);
        double measureZA = motionZA + noisePosDistribution(e);

        Pose curMeasurePoseA = fromRPYXYZ(measureRollA, measurePitchA, measureYawA, measureXA, measureYA, measureZA);

        Pose curMotionPoseB = curMotionPoseA * transB2A;

        double motionRollB = 0, motionPitchB = 0, motionYawB = 0;
        double motionXB = 0, motionYB = 0, motionZB = 0;
        toRPYXYZ(curMotionPoseB, motionRollB, motionPitchB, motionYawB, motionXB, motionYB, motionZB);

        double measureRollB = motionRollB + noiseAngleDistribution(e);
        double measurePitchB = motionPitchB + noiseAngleDistribution(e);
        double measureYawB = motionYawB + noiseAngleDistribution(e);
        double measureXB = motionXB + noisePosDistribution(e);
        double measureYB = motionYB + noisePosDistribution(e);
        double measureZB = motionZB + noisePosDistribution(e);

        Pose curMeasurePoseB = fromRPYXYZ(measureRollB, measurePitchB, measureYawB, measureXB, measureYB, measureZB);
        // Pose curMeasurePoseB = curMotionPoseB;

        A.push_back(preMeasurePoseA.inverse() * curMeasurePoseA);
        B.push_back(preMeasurePoseB.inverse() * curMeasurePoseB);

        preMotionPoseA = curMotionPoseA;
        preMotionPoseB = curMotionPoseB;
        preMeasurePoseA = curMeasurePoseA;
        preMeasurePoseB = curMeasurePoseB;
    }
}

TEST(CALIBRATION_HANDEYE, RT_from_to_rpyxyz)
{
    double initRoll = M_PI * 0.2, initPitch = M_PI * 0.4, initYaw = M_PI * 0.25;
    double initX = 13.2, initY = 1.99, initZ = 8;
    Pose initCalib = fromRPYXYZ(initRoll, initPitch, initYaw, initX, initY, initZ);

    double navyRoll = 0, navyPitch = 0, navyYaw = 0;
    double navyX = 0, navyY = 0, navyZ = 0;
    toRPYXYZ(initCalib, navyRoll, navyPitch, navyYaw, navyX, navyY, navyZ);

    std::cout << initRoll << "\t" << navyRoll << std::endl;
    std::cout << initPitch << "\t" << navyPitch << std::endl;
    std::cout << initYaw << "\t" << navyYaw << std::endl;
    std::cout << initX << "\t" << navyX << std::endl;
    std::cout << initY << "\t" << navyY << std::endl;
    std::cout << initZ << "\t" << navyZ << std::endl;
}

TEST(CALIBRATION_HANDEYE, eigen_rodrigues)
{
    double initRoll = M_PI * 0.2, initPitch = M_PI * 0.4, initYaw = M_PI * 0.25;
    double initX = 13.2, initY = 1.99, initZ = 8;
    Transform initCalib = fromRPYXYZ(initRoll, initPitch, initYaw, initX, initY, initZ);

    Eigen::Matrix3d initRot = initCalib.block(0, 0, 3, 3);
    Eigen::AngleAxisd initAngleAxis;
    initAngleAxis.fromRotationMatrix(initRot);

    double initAngle = initAngleAxis.angle();
    Eigen::Vector3d initAxis = initAngleAxis.axis();

    EXPECT_EQ(1, initAxis.norm());

    Eigen::Vector3d initRotVec = 2 * std::sin(initAngle / 2) * initAxis;
    std::cout << initRotVec.transpose() << std::endl;

    Eigen::Matrix3d initSkew;
    initSkew << 0, -initRotVec(2), initRotVec(1),
        initRotVec(2), 0, -initRotVec(0),
        -initRotVec(1), initRotVec(0), 0;
}

TEST(CALIBRATION_HANDEYE, tsai_without_noise)
{
    double initRoll = M_PI * 0.2, initPitch = M_PI * 0.4, initYaw = M_PI * 0.25;
    double initX = 13.2, initY = 1.99, initZ = 8;
    Pose initCalib = fromRPYXYZ(initRoll, initPitch, initYaw, initX, initY, initZ);

    PoseVec poseVecA, poseVecB;
    initRandomMotionWithoutNoise(poseVecA, poseVecB, initCalib);

    TsaiCalibration tsaiCalibration;
    Pose tsaiCalib = tsaiCalibration.solve(poseVecA, poseVecB);
    double tsaiRoll = 0, tsaiPitch = 0, tsaiYaw = 0;
    double tsaiX = 0, tsaiY = 0, tsaiZ = 0;
    toRPYXYZ(tsaiCalib, tsaiRoll, tsaiPitch, tsaiYaw, tsaiX, tsaiY, tsaiZ);

    std::cout << initRoll << "\t" << tsaiRoll << std::endl;
    std::cout << initPitch << "\t" << tsaiPitch << std::endl;
    std::cout << initYaw << "\t" << tsaiYaw << std::endl;
    std::cout << initX << "\t" << tsaiX << std::endl;
    std::cout << initY << "\t" << tsaiY << std::endl;
    std::cout << initZ << "\t" << tsaiZ << std::endl;
}

TEST(CALIBRATION_HANDEYE, tsai_with_noise)
{
    double initRoll = M_PI * 0.2, initPitch = M_PI * 0.4, initYaw = M_PI * 0.25;
    double initX = 13.2, initY = 1.99, initZ = 8;
    Pose initCalib = fromRPYXYZ(initRoll, initPitch, initYaw, initX, initY, initZ);

    PoseVec poseVecA, poseVecB;
    initRandomMotionWithNoise(poseVecA, poseVecB, initCalib);

    TsaiCalibration tsaiCalibration;
    Pose tsaiCalib = tsaiCalibration.solve(poseVecA, poseVecB);
    double tsaiRoll = 0, tsaiPitch = 0, tsaiYaw = 0;
    double tsaiX = 0, tsaiY = 0, tsaiZ = 0;
    toRPYXYZ(tsaiCalib, tsaiRoll, tsaiPitch, tsaiYaw, tsaiX, tsaiY, tsaiZ);

    std::cout << initRoll << "\t" << tsaiRoll << std::endl;
    std::cout << initPitch << "\t" << tsaiPitch << std::endl;
    std::cout << initYaw << "\t" << tsaiYaw << std::endl;
    std::cout << initX << "\t" << tsaiX << std::endl;
    std::cout << initY << "\t" << tsaiY << std::endl;
    std::cout << initZ << "\t" << tsaiZ << std::endl;
}

TEST(CALIBRATION_HANDEYE, navy_without_noise)
{
    double initRoll = M_PI * 0.2, initPitch = M_PI * 0.4, initYaw = M_PI * 0.25;
    double initX = 13.2, initY = 1.99, initZ = 8;
    Pose initCalib = fromRPYXYZ(initRoll, initPitch, initYaw, initX, initY, initZ);

    PoseVec poseVecA, poseVecB;
    initRandomMotionWithoutNoise(poseVecA, poseVecB, initCalib);

    NavyCalibration navyCalibration;
    Pose navyCalib = navyCalibration.solve(poseVecA, poseVecB);
    double navyRoll = 0, navyPitch = 0, navyYaw = 0;
    double navyX = 0, navyY = 0, navyZ = 0;
    toRPYXYZ(navyCalib, navyRoll, navyPitch, navyYaw, navyX, navyY, navyZ);

    std::cout << initRoll << "\t" << navyRoll << std::endl;
    std::cout << initPitch << "\t" << navyPitch << std::endl;
    std::cout << initYaw << "\t" << navyYaw << std::endl;
    std::cout << initX << "\t" << navyX << std::endl;
    std::cout << initY << "\t" << navyY << std::endl;
    std::cout << initZ << "\t" << navyZ << std::endl;
}

TEST(CALIBRATION_HANDEYE, navy_with_noise)
{
    double initRoll = M_PI * 0.2, initPitch = M_PI * 0.4, initYaw = M_PI * 0.25;
    double initX = 13.2, initY = 1.99, initZ = 8;
    Transform initCalib = fromRPYXYZ(initRoll, initPitch, initYaw, initX, initY, initZ);

    TransVec transVecA, transVecB;
    initRandomMotionWithNoise(transVecA, transVecB, initCalib);

    NavyCalibration navyCalibration;
    Transform navyCalib = navyCalibration.solve(transVecA, transVecB);

    double navyRoll = 0, navyPitch = 0, navyYaw = 0;
    double navyX = 0, navyY = 0, navyZ = 0;
    toRPYXYZ(navyCalib, navyRoll, navyPitch, navyYaw, navyX, navyY, navyZ);

    std::cout << initRoll << "\t" << navyRoll << std::endl;
    std::cout << initPitch << "\t" << navyPitch << std::endl;
    std::cout << initYaw << "\t" << navyYaw << std::endl;
    std::cout << initX << "\t" << navyX << std::endl;
    std::cout << initY << "\t" << navyY << std::endl;
    std::cout << initZ << "\t" << navyZ << std::endl;
}

