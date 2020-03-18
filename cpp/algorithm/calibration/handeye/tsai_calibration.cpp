#include "tsai_calibration.h"

TsaiCalibration::TsaiCalibration()
{
}

TsaiCalibration::~TsaiCalibration()
{
}

Eigen::MatrixXd svdInv(const Eigen::MatrixXd &mat)
{
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(mat, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::MatrixXd svdU = svd.matrixU();
    Eigen::MatrixXd svdV = svd.matrixV();
    Eigen::Vector3d svdS = svd.singularValues();
    Eigen::Matrix3d svdInvS = Eigen::Matrix3d::Zero();
    for (int i = 0; i < 3; i++)
    {
        svdInvS(i, i) = (svdS(i) < 0.001) ? 0 : 1.0 / svdS(i);
    }
    // std::cout<<"Svd U =\n"<<svdU<<std::endl;
    // std::cout<<"Svd V =\n"<<svdV<<std::endl;
    // std::cout<<"Svd S = "<<svdS.transpose()<<std::endl;
    // std::cout<<"Svd Inv S =\n"<<svdInvS<<std::endl;
    Eigen::MatrixXd matInv = svdV * svdInvS * svdU.transpose();
    return matInv;
}

TsaiCalibration::Transform TsaiCalibration::solve(const TsaiCalibration::TransformVec &A, const TsaiCalibration::TransformVec &B)
{
    const int num = A.size();
    Eigen::MatrixXd aRot(3 * num, 3);
    aRot.setZero();
    Eigen::VectorXd bRot(3 * num);
    bRot.setZero();

    for (size_t i = 0; i < A.size(); i++)
    {
        Eigen::Matrix3d rotA = A[i].block(0, 0, 3, 3);
        Eigen::Matrix3d rotB = B[i].block(0, 0, 3, 3);

        Eigen::AngleAxisd angleAxisA;
        angleAxisA.fromRotationMatrix(rotA);
        Eigen::AngleAxisd angleAxisB;
        angleAxisB.fromRotationMatrix(rotB);

        Eigen::Vector3d rotVecA = 2 * std::sin(angleAxisA.angle() / 2) * angleAxisA.axis();
        Eigen::Vector3d rotVecB = 2 * std::sin(angleAxisB.angle() / 2) * angleAxisB.axis();
        Eigen::Vector3d rotVecAB = rotVecA + rotVecB;

        Eigen::Matrix3d skewAB;
        skewAB << 0, -rotVecAB(2), rotVecAB(1),
            rotVecAB(2), 0, -rotVecAB(0),
            -rotVecAB(1), rotVecAB(0), 0;

        Eigen::Vector3d tmpb = rotVecB - rotVecA;

        aRot.block(3 * i, 0, 3, 3) = skewAB;
        bRot(3 * i) = tmpb(0);
        bRot(3 * i + 1) = tmpb(1);
        bRot(3 * i + 2) = tmpb(2);
    }

    Eigen::MatrixXd aRotInv = svdInv(aRot);
    Eigen::Vector3d rotPrime = aRotInv * bRot;
    Eigen::Vector3d rotVec = 2 * rotPrime / std::sqrt(1 + rotPrime.norm() * rotPrime.norm());
    Eigen::Matrix3d rotSkew;
    rotSkew << 0, -rotVec(2), rotVec(1),
        rotVec(2), 0, -rotVec(0),
        -rotVec(1), rotVec(0), 0;
    Eigen::Matrix3d rot = (1 - rotVec.norm() * rotVec.norm() / 2) * Eigen::Matrix3d::Identity() +
                          0.5 * (rotVec * rotVec.transpose() + std::sqrt(4 - rotVec.norm() * rotVec.norm()) * rotSkew);

    Eigen::MatrixXd aTrs(3 * num, 3);
    aTrs.setZero();
    Eigen::VectorXd bTrs(3 * num);
    bTrs.setZero();
    for (size_t i = 0; i < A.size(); i++)
    {
        Eigen::Matrix3d tmpARot = A[i].block(0, 0, 3, 3);
        Eigen::Vector3d tmpATrs = A[i].block(0, 3, 3, 1);
        Eigen::Matrix3d tmpBRot = B[i].block(0, 0, 3, 3);
        Eigen::Vector3d tmpBTrs = B[i].block(0, 3, 3, 1);

        Eigen::Matrix3d tmpA = tmpARot - Eigen::Matrix3d::Identity();
        Eigen::Vector3d tmpb = rot * tmpBTrs - tmpATrs;

        aTrs.block(3 * i, 0, 3, 3) = tmpA;
        bTrs(3 * i) = tmpb(0);
        bTrs(3 * i + 1) = tmpb(1);
        bTrs(3 * i + 2) = tmpb(2);
    }
    Eigen::MatrixXd aTrsInv = svdInv(aTrs);
    Eigen::Vector3d t = aTrsInv * bTrs;

    // Eigen::Matrix3d C = Eigen::Matrix3d::Zero();
    // Eigen::Vector3d d = Eigen::Vector3d::Zero();
    // for (size_t i = 0; i < A.size(); i++)
    // {
    //     Transform a = A[i], b = B[i];
    //     Rotation rA = a.block(0, 0, 3, 3);
    //     Rotation rB = a.block(0, 0, 3, 3);
    //     Translation tA = a.block(0, 3, 3, 1);
    //     Translation tB = b.block(0, 3, 3, 1);

    //     C.noalias() += (Eigen::Matrix3d::Identity() - rA);
    //     d.noalias() += (tA - rot * tB);
    // }
    // Eigen::Vector3d t = (C.transpose() * C).inverse() * (C.transpose() * d);

    Eigen::Matrix4d trans = Eigen::Matrix4d::Identity();
    trans.block(0, 0, 3, 3) = rot;
    trans.block(0, 3, 3, 1) = t;
    return trans;
}