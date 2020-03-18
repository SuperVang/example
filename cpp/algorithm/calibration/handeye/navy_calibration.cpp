#include "navy_calibration.h"

NavyCalibration::NavyCalibration()
{
}

NavyCalibration::~NavyCalibration()
{
}

Eigen::Vector3d logMat(const Eigen::Matrix3d &mat)
{
    double fi = std::acos((mat.trace() - 1) / 2.0);
    Eigen::Matrix3d w = fi / (2 * std::sin(fi)) * (mat - mat.transpose());
    Eigen::Vector3d result;
    result << w(2, 1), w(0, 2), w(1, 0);
    return result;
}

NavyCalibration::Transform NavyCalibration::solve(const NavyCalibration::TransformVec &A, const NavyCalibration::TransformVec &B)
{
    assert(A.size() == B.size() && A.size() >= 2);

    Rotation rot = Rotation::Identity();
    Translation t = Translation::Zero();
    if (A.size() == 2)
    {
        Transform a1 = A[0], a2 = A[1], b1 = B[0], b2 = B[1];
        Rotation rA1 = a1.block(0, 0, 3, 3);
        Rotation rA2 = a2.block(0, 0, 3, 3);
        Rotation rB1 = b1.block(0, 0, 3, 3);
        Rotation rB2 = b2.block(0, 0, 3, 3);

        Coloum alpha1 = logMat(rA1);
        Coloum alpha2 = logMat(rA2);
        Coloum beta1 = logMat(rB1);
        Coloum beta2 = logMat(rB2);

        Eigen::Matrix3d M = Eigen::Matrix3d::Zero();
        M.col(0) = alpha1;
        M.col(1) = alpha2;
        Eigen::Vector3d alphaCross = alpha1.cross(alpha2);
        M.col(2) = alphaCross;

        Eigen::Matrix3d N = Eigen::Matrix3d::Zero();
        N.col(0) = beta1;
        N.col(1) = beta2;
        Eigen::Vector3d betaCross = beta1.cross(beta2);
        N.col(2) = betaCross;

        rot = M * N.inverse();
    }
    else
    {
        Eigen::Matrix3d M = Eigen::Matrix3d::Zero();
        Eigen::Matrix3d N = Eigen::Matrix3d::Zero();
        for (size_t i = 0; i < A.size(); i++)
        {
            Transform a = A[i], b = B[i];
            Rotation rA = a.block(0, 0, 3, 3);
            Rotation rB = b.block(0, 0, 3, 3);

            Coloum alpha = logMat(rA);
            Coloum beta = logMat(rB);

            M.noalias() += beta * alpha.transpose();

            Eigen::AngleAxisd angleAxisdAlpha(rA), angleAxisdBeta(rB);
            Coloum alpha2 = angleAxisdAlpha.axis();
            Coloum beta2 = angleAxisdBeta.axis();
        }

        Eigen::Matrix3d MtM = M.transpose() * M;

        Eigen::EigenSolver<Eigen::Matrix3d> eig(MtM);
        Eigen::Matrix3d eigVal = eig.pseudoEigenvalueMatrix();
        Eigen::Matrix3d eigVec = eig.pseudoEigenvectors();

        Eigen::Matrix3d eigMat = Eigen::Matrix3d::Zero();
        eigMat(0, 0) = 1 / sqrt(eigVal(0, 0));
        eigMat(1, 1) = 1 / sqrt(eigVal(1, 1));
        eigMat(2, 2) = 1 / sqrt(eigVal(2, 2));

        rot = eigVec * eigMat * eigVec.inverse() * M.transpose();
    }

    Eigen::Matrix3d C = Eigen::Matrix3d::Zero();
    Eigen::Vector3d d = Eigen::Vector3d::Zero();
    for (size_t i = 0; i < A.size(); i++)
    {
        Transform a = A[i], b = B[i];
        Rotation rA = a.block(0, 0, 3, 3);
        Rotation rB = a.block(0, 0, 3, 3);
        Translation tA = a.block(0, 3, 3, 1);
        Translation tB = b.block(0, 3, 3, 1);

        C.noalias() += (Eigen::Matrix3d::Identity() - rA);
        d.noalias() += (tA - rot * tB);
    }
    t = (C.transpose() * C).inverse() * (C.transpose() * d);

    Transform transform = Transform::Identity();
    transform.block(0, 0, 3, 3) = rot;
    transform.block(0, 3, 3, 1) = t;
    return transform;
}
