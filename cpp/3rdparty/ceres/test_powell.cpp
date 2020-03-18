//
// Created by books on 19-12-6.
//

#include <iostream>
#include <gtest/gtest.h>

#include <Eigen/Core>
#include <ceres/ceres.h>

struct AutoCostFunctor1
{
    template<typename T>
    bool operator()(const T *const x1, const T *const x2, T *residual) const
    {
        residual[0] = x1[0] + T(10.0) * x2[0];
        return true;
    }
};

struct AutoCostFunctor2
{
    template<typename T>
    bool operator()(const T *const x3, const T *const x4, T *residual) const
    {
        residual[0] = T(std::sqrt(5.0)) * (x3[0] - x4[0]);
        return true;
    }
};

struct AutoCostFunctor3
{
    template<typename T>
    bool operator()(const T *const x2, const T *const x3, T *residual) const
    {
        residual[0] = (x2[0] - T(2.0) * x3[0]) * (x2[0] - T(2.0) * x3[0]);
        return true;
    }
};

struct AutoCostFunctor4
{
    template<typename T>
    bool operator()(const T *const x1, const T *const x4, T *residual) const
    {
        residual[0] = T(std::sqrt(10.0)) * (x1[0] - x4[0]) * (x1[0] - x4[0]);
        return true;
    }
};

TEST(CERES_POWELL, one_by_one)
{
    double initial_x1 = 1.0, initial_x2 = 5.0, initial_x3 = -10.0, initial_x4 = -2.4;
    double x1 = initial_x1, x2 = initial_x2, x3 = initial_x3, x4 = initial_x4;

    ceres::Problem problem;

    ceres::CostFunction *cost_func1 = new ceres::AutoDiffCostFunction<AutoCostFunctor1, 1, 1, 1>(new AutoCostFunctor1);
    ceres::CostFunction *cost_func2 = new ceres::AutoDiffCostFunction<AutoCostFunctor2, 1, 1, 1>(new AutoCostFunctor2);
    ceres::CostFunction *cost_func3 = new ceres::AutoDiffCostFunction<AutoCostFunctor3, 1, 1, 1>(new AutoCostFunctor3);
    ceres::CostFunction *cost_func4 = new ceres::AutoDiffCostFunction<AutoCostFunctor4, 1, 1, 1>(new AutoCostFunctor4);
    problem.AddResidualBlock(cost_func1, nullptr, &x1, &x2);
    problem.AddResidualBlock(cost_func2, nullptr, &x3, &x4);
    problem.AddResidualBlock(cost_func3, nullptr, &x2, &x3);
    problem.AddResidualBlock(cost_func4, nullptr, &x1, &x4);

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = false;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    EXPECT_LE(std::abs(x1), 0.01);
    EXPECT_LE(std::abs(x2), 0.01);
    EXPECT_LE(std::abs(x3), 0.01);
    EXPECT_LE(std::abs(x4), 0.01);
}

struct AutoCostFunctorAll
{

    template<typename T>
    bool operator()(const T *const x, T *residual) const
    {
        residual[0] = x[0] + T(10.0) * x[1];
        residual[1] = T(std::sqrt(5.0)) * (x[2] - x[3]);
        residual[2] = (x[1] - T(2.0) * x[2]) * (x[1] - T(2.0) * x[2]);
        residual[3] = T(std::sqrt(10.0)) * (x[0] - x[3]) * (x[0] - x[3]);
        return true;
    }
};

TEST(CERES_POWELL, all_in_one)
{
    double initial_x1 = 1.0, initial_x2 = 5.0, initial_x3 = -10.0, initial_x4 = -2.4;
    double *x = new double[4];
    x[0] = initial_x1;
    x[1] = initial_x2;
    x[2] = initial_x3;
    x[3] = initial_x4;

    ceres::Problem problem;
    ceres::CostFunction *cost_func = new ceres::AutoDiffCostFunction<AutoCostFunctorAll, 4, 4>(new AutoCostFunctorAll);
    problem.AddResidualBlock(cost_func, nullptr, x);

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = false;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    EXPECT_LE(std::abs(x[0]), 0.01);
    EXPECT_LE(std::abs(x[1]), 0.01);
    EXPECT_LE(std::abs(x[2]), 0.01);
    EXPECT_LE(std::abs(x[3]), 0.01);
}