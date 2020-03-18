#ifndef NAVY_CALIBRATION_H
#define NAVY_CALIBRATION_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Eigenvalues>

class NavyCalibration
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    typedef Eigen::Matrix3d Rotation;
    typedef Eigen::Vector3d Translation;
    typedef Eigen::Vector3d Coloum;
    typedef Eigen::Matrix4d Transform;
    typedef std::vector<Transform, Eigen::aligned_allocator<Transform>> TransformVec;

public:
    NavyCalibration();
    ~NavyCalibration();

    Transform solve(const TransformVec &A, const TransformVec &B);

private:
};

#endif