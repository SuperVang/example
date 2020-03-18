//
// Created by books on 19-12-6.
//

#include <iostream>
#include <gtest/gtest.h>

#include <Eigen/Core>
#include <ceres/ceres.h>

/// cost(x)=10+x^2-2*x
struct AutoCostFunctor
{
    template<typename T>
    bool operator()(const T *const x, T *residual) const
    {
        residual[0] = T(10.0) + x[0] * x[0] - T(2.0) * x[0];
        return true;
    }
};

TEST(CERES_DERIVATIVE, auto_diff_cost)
{
    double initial_x = 10.0;
    double x = initial_x;

    ceres::Problem problem;

    ceres::CostFunction *cost_func = new ceres::AutoDiffCostFunction<AutoCostFunctor, 1, 1>(new AutoCostFunctor);
    problem.AddResidualBlock(cost_func, nullptr, &x);

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = false;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    EXPECT_LE(std::abs(x - 1.0), 0.01);

}

struct NumericCostFunctor
{
    bool operator()(const double *const x, double *residual) const
    {
        residual[0] = 10.0 + x[0] * x[0] - 2.0 * x[0];
        return true;
    }
};

TEST(CERES_DERIVATIVE, numeric_diff_cost)
{
    double initial_x = 10.0;
    double x = initial_x;

    ceres::Problem problem;

    ceres::CostFunction *cost_func = new ceres::NumericDiffCostFunction<NumericCostFunctor, ceres::CENTRAL, 1, 1>(
            new NumericCostFunctor);
    problem.AddResidualBlock(cost_func, nullptr, &x);

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = false;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    EXPECT_LE(std::abs(x - 1.0), 0.01);
}

class AnalyticCostFunctor : public ceres::SizedCostFunction<1, 1>
{
public:
    virtual ~AnalyticCostFunctor()
    {}

    virtual bool Evaluate(double const *const *parameters, double *residuals, double **jacobians) const
    {
        const double x = parameters[0][0];
        residuals[0] = 10.0 + x * x - 2.0 * x;

        if (jacobians != nullptr && jacobians[0] != nullptr)
        {
            jacobians[0][0] = 2.0 * x - 2.0;
        }
        return true;
    }
};

TEST(CERES_DERIVATIVE, analytic_diff_cost)
{
    double initial_x = 10.0;
    double x = initial_x;

    ceres::Problem problem;

    ceres::CostFunction *cost_func = new AnalyticCostFunctor();
    problem.AddResidualBlock(cost_func, nullptr, &x);

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = false;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    EXPECT_LE(std::abs(x - 1.0), 0.01);
}
