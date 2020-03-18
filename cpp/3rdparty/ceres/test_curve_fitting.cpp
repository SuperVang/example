//
// Created by books on 19-12-6.
//

#include <iostream>
#include <random>
#include <cmath>
#include <gtest/gtest.h>

#include <Eigen/Core>
#include <ceres/ceres.h>

/// y=exp(ax+b)
/// cost=y-exp(ax+b)
struct ExponentialResidual
{

    ExponentialResidual(double x, double y) : m_x(x), m_y(y)
    {}

    template<typename T>
    bool operator()(const T * const theta, T *residual) const
    {
        residual[0] = T(m_y) - exp(theta[0] * T(m_x) + theta[1]);
        return true;
    }

private:
    const double m_x;
    const double m_y;
};

TEST(CERES_CURVE_FITTING, exp_filtting)
{
    std::default_random_engine e(std::time(nullptr));
    std::normal_distribution<double> normal_dis(0, 0.1);

    double *real_theta = new double[2];
    real_theta[0] = 2.5;
    real_theta[1] = 3.3;
    double *theta = new double[2];
    theta[0] = real_theta[0]+normal_dis(e);
    theta[1] = real_theta[1]+normal_dis(e);

    ceres::Problem problem;
    for (int i = 0; i < 10; i++)
    {
        double x = i;
        double y = std::exp(real_theta[0] * i + real_theta[1]) + normal_dis(e);
        ceres::CostFunction *cost_func = new ceres::AutoDiffCostFunction<ExponentialResidual, 1, 2>(
                new ExponentialResidual(x, y));
        problem.AddResidualBlock(cost_func, new ceres::CauchyLoss(0.5), theta);
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = false;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

//    std::cout<<summary.BriefReport()<<std::endl;


    EXPECT_LE(std::abs(real_theta[0] - theta[0]), 0.01);
    EXPECT_LE(std::abs(real_theta[1] - theta[1]), 0.01);

}