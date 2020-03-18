//
// Created by books on 19-12-9.
//

#include <iostream>
#include <random>
#include <cmath>
#include <gtest/gtest.h>

#include <Eigen/Core>
#include <ceres/ceres.h>
#include <ceres/rotation.h>

// Normalizes the angle in radians between [-pi and pi).
template<typename T>
inline T NormalizeAngle(const T &angle_radians)
{
    // Use ceres::floor because it is specialized for double and Jet types.
    T two_pi(2.0 * M_PI);
    return angle_radians - two_pi * ceres::floor((angle_radians + T(M_PI)) / two_pi);
}

class AngleLocalParameterization
{
public:
    template<typename T>
    bool operator()(const T *theta_radians, const T *delta_theta_radians, T *theta_radians_plus_delta) const
    {
        *theta_radians_plus_delta = NormalizeAngle(*theta_radians + *delta_theta_radians);

        return true;
    }

    static ceres::LocalParameterization *Create()
    {
        return (new ceres::AutoDiffLocalParameterization<AngleLocalParameterization, 1, 1>);
    }
};

struct Transform2DError
{
public:

    Transform2DError(const Eigen::Vector2d &origin_pt, const Eigen::Vector2d &measure_pt) : m_origin_pt(origin_pt),
                                                                                            m_measure_pt(measure_pt)
    {}

    template<typename T>
    bool operator()(const T *const yaw, const T *const translation, T *residuals) const
    {
        ///
//        Eigen::Map<const Eigen::Rotation2D<T>> q(yaw[0]);
        Eigen::Rotation2D<T> q(yaw[0]);
        Eigen::Map<const Eigen::Matrix<T, 2, 1>> p(translation);

        Eigen::Transform<T, 2, Eigen::Isometry> transform;
        transform.setIdentity();
        transform.rotate(q);
        transform.translate(p);

        ///
        Eigen::Matrix<T, 2, 1> transform_pt = transform * m_origin_pt.cast<T>();
        residuals[0] = transform_pt(0) - T(m_measure_pt(0));
        residuals[1] = transform_pt(1) - T(m_measure_pt(1));

        return true;
    }

    static ceres::CostFunction *Create(const Eigen::Vector2d &origin_pt, const Eigen::Vector2d &measure_pt)
    {
        return (new ceres::AutoDiffCostFunction<Transform2DError, 2, 1, 2>(
                new Transform2DError(origin_pt, measure_pt)));
    }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
    const Eigen::Vector2d m_origin_pt;
    const Eigen::Vector2d m_measure_pt;

};

TEST(CERES_TRANSFORM, isometry_2d)
{
    std::cout << std::endl;

    std::default_random_engine e(std::time(nullptr));
    std::uniform_real_distribution<> xyzUniform(15.0, 50.0);
    std::normal_distribution<double> angleNormal(0.0, 25.0);
    std::normal_distribution<double> xyzNormal(0.0, 2.0);

    double init_yaw = 45.0 * M_PI / 180.0;
    Eigen::Vector2d init_translation(1.0, 3.0);
    Eigen::Isometry2d init_transform = Eigen::Isometry2d::Identity();
    init_transform.rotate(Eigen::Rotation2Dd(init_yaw));
    init_transform.translate(init_translation);

    double yaw = (45.0 + angleNormal(e)) * M_PI / 180.0;
    Eigen::Vector2d translation = init_translation + Eigen::Vector2d(xyzNormal(e), xyzNormal(e));

    std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> origin_pts;
    std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> measure_pts;
    for (int i = 0; i < 10; i++)
    {
        Eigen::Vector2d tmp_origin(xyzUniform(e), xyzUniform(e));
        Eigen::Vector2d tmp_measure = init_transform * tmp_origin + Eigen::Vector2d(xyzNormal(e), xyzNormal(e));
        origin_pts.push_back(tmp_origin);
        measure_pts.push_back(tmp_measure);
    }

    ceres::Problem problem;
    ceres::LossFunction *loss_func = new ceres::CauchyLoss(0.4);
    ceres::LocalParameterization *angle_local_para = AngleLocalParameterization::Create();
    for (size_t i = 0; i < origin_pts.size(); i++)
    {
        ceres::CostFunction *cost_func = Transform2DError::Create(origin_pts[i], measure_pts[i]);
        problem.AddResidualBlock(cost_func, loss_func, &yaw, translation.data());
        problem.SetParameterization(&yaw, angle_local_para);
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::SPARSE_NORMAL_CHOLESKY;
    options.max_num_iterations = 200;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.BriefReport() << std::endl;

    std::cout << "Init Rotation " << init_yaw << std::endl;
    std::cout << "Rotation " << yaw << std::endl;

    std::cout << "Init Translation " << init_translation.transpose() << std::endl;
    std::cout << "Translation " << translation.transpose() << std::endl;
}

struct Transform3DError
{
public:

    Transform3DError(const Eigen::Vector3d &origin_pt, const Eigen::Vector3d &measure_pt) : m_origin_pt(origin_pt),
                                                                                            m_measure_pt(measure_pt)
    {}

    template<typename T>
    bool operator()(const T *const quaternion, const T *const translation, T *residuals) const
    {
        ///
        Eigen::Map<const Eigen::Quaternion<T>> q(quaternion);
        Eigen::Map<const Eigen::Matrix<T, 3, 1>> p(translation);

        ///
        Eigen::Transform<T, 3, Eigen::Isometry> transform;
        transform.setIdentity();
        transform.rotate(q);
        transform.translate(p);

        ///
        Eigen::Matrix<T, 3, 1> transform_pt = transform * m_origin_pt.cast<T>();
        residuals[0] = transform_pt(0) - T(m_measure_pt(0));
        residuals[1] = transform_pt(1) - T(m_measure_pt(1));
        residuals[2] = transform_pt(2) - T(m_measure_pt(2));

        return true;
    }

    static ceres::CostFunction *Create(const Eigen::Vector3d &origin_pt, const Eigen::Vector3d &measure_pt)
    {
        return (new ceres::AutoDiffCostFunction<Transform3DError, 3, 4, 3>(
                new Transform3DError(origin_pt, measure_pt)));
    }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
    const Eigen::Vector3d m_origin_pt;
    const Eigen::Vector3d m_measure_pt;
};

Eigen::Quaterniond buildFromRPY(double roll, double pitch, double yaw)
{
    Eigen::Matrix3d r;
    r = Eigen::AngleAxisd(yaw * M_PI / 180.0, Eigen::Vector3d::UnitZ()) *
        Eigen::AngleAxisd(pitch * M_PI / 180.0, Eigen::Vector3d::UnitY()) *
        Eigen::AngleAxisd(roll * M_PI / 180.0, Eigen::Vector3d::UnitX());
    return Eigen::Quaterniond(r);
}

TEST(CERES_TRANSFORM, isometry_3d)
{
    std::cout << std::endl;

    std::default_random_engine e(std::time(nullptr));
    std::uniform_real_distribution<> xyzUniform(15.0, 50.0);
    std::normal_distribution<double> angleNormal(0.0, 25.0);
    std::normal_distribution<double> xyzNormal(0.0, 2.0);

    Eigen::Quaterniond init_quaternion = buildFromRPY(0.0, 0.0, 45);
    Eigen::Vector3d init_translation(1.0, 3.0, 2.0);
    Eigen::Isometry3d init_transform = Eigen::Isometry3d::Identity();
    init_transform.rotate(init_quaternion);
    init_transform.translate(init_translation);

    Eigen::Quaterniond quaternion = buildFromRPY(angleNormal(e), angleNormal(e), 45 + angleNormal(e));
    Eigen::Vector3d translation = init_translation + Eigen::Vector3d(xyzNormal(e), xyzNormal(e), xyzNormal(e));

    std::vector<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> origin_pts;
    std::vector<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> measure_pts;
    for (int i = 0; i < 10; i++)
    {
        Eigen::Vector3d tmp_origin(xyzUniform(e), xyzUniform(e), xyzUniform(e));
        Eigen::Vector3d tmp_measure =
                init_transform * tmp_origin + Eigen::Vector3d(xyzNormal(e), xyzNormal(e), xyzNormal(e));
        origin_pts.push_back(tmp_origin);
        measure_pts.push_back(tmp_measure);
    }

    ceres::Problem problem;
    ceres::LossFunction *loss_func = new ceres::CauchyLoss(0.4);
    ceres::LocalParameterization *quaternion_local_para = new ceres::EigenQuaternionParameterization();
    for (size_t i = 0; i < origin_pts.size(); i++)
    {
        ceres::CostFunction *cost_func = Transform3DError::Create(origin_pts[i], measure_pts[i]);
        problem.AddResidualBlock(cost_func, loss_func, quaternion.coeffs().data(), translation.data());
        problem.SetParameterization(quaternion.coeffs().data(), quaternion_local_para);
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::SPARSE_NORMAL_CHOLESKY;
    options.max_num_iterations = 200;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.BriefReport() << std::endl;

    std::cout << "Init Rotation \n" << init_quaternion.matrix() << std::endl;
    std::cout << "Rotation \n" << quaternion.matrix() << std::endl;

    std::cout << "Init Translation " << init_translation.transpose() << std::endl;
    std::cout << "Translation " << translation.transpose() << std::endl;
}