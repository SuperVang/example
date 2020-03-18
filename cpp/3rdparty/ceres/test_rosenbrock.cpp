//
// Created by books on 19-12-8.
//

#include <iostream>
#include <gtest/gtest.h>

#include <Eigen/Core>
#include <ceres/ceres.h>

/// f(x,y)=(1-x)^2+100.0*(y-x^2)^2

struct RosenBrock2Auto
{
	template<typename T>
	bool operator() (const T *const parameter, T *residual) const
	{
		residual[0] = (T(1.0) - parameter[0]) * (T(1.0) - parameter[0]) + T(100.0) * (parameter[1] - parameter[0] * parameter[0]) * (parameter[1] - parameter[0] * parameter[0]);
		return true;
	}
};

TEST(CERES_ROSENBROCK, auto_diff_2)
{
	double *para = new double[2];
	para[0] = 3.0;
	para[1] = 5.0;
	
	ceres::Problem problem;
	ceres::CostFunction *cost_func = new ceres::AutoDiffCostFunction<RosenBrock2Auto, 1, 2>(new RosenBrock2Auto);
	problem.AddResidualBlock(cost_func, nullptr, para);
	
	ceres::Solver::Options options;
	options.linear_solver_type = ceres::DENSE_QR;
	options.minimizer_progress_to_stdout = false;
	options.max_num_iterations = 100;
	ceres::Solver::Summary summary;
	ceres::Solve(options, &problem, &summary);
	
	std::cout << summary.BriefReport() << std::endl;
}

class RosenBrock2Analytic : public ceres::SizedCostFunction<1, 2>
{
public:
	virtual ~RosenBrock2Analytic () {}
	
	virtual bool Evaluate (double const *const *parameter, double *residual, double **jacobians) const
	{
		residual[0] = (1.0 - parameter[0][0]) * (1.0 - parameter[0][0]) + 100.0 * (parameter[0][1] - parameter[0][0] * parameter[0][0]) * (parameter[0][1] - parameter[0][0] * parameter[0][0]);
		
		if (jacobians != nullptr && jacobians[0] != nullptr)
		{
			jacobians[0][0] = -2.0 * (1.0 - parameter[0][0]) - 200.0 * (parameter[0][1] - parameter[0][0] * parameter[0][0]) * 2.0 * parameter[0][0];
			jacobians[0][1] = 200.0 * (parameter[0][1] - parameter[0][0] * parameter[0][0]);
		}
		return true;
	}
};

TEST(CERES_ROSENBLOCK, use_analytic_diff)
{
	double *para = new double[2];
	para[0] = 3.0;
	para[1] = 5.0;
	
	ceres::Problem problem;
	
	ceres::CostFunction *cost_func = new RosenBrock2Analytic();
	problem.AddResidualBlock(cost_func, nullptr, para);
	
	ceres::Solver::Options options;
	options.linear_solver_type = ceres::DENSE_QR;
	options.minimizer_progress_to_stdout = true;
	ceres::Solver::Summary summary;
	ceres::Solve(options, &problem, &summary);
	
	std::cout << summary.BriefReport() << std::endl;
}

/// inherit from CostFunction
class RosenBlock2 : public ceres::FirstOrderFunction
{
public:
	virtual bool Evaluate (const double *parameters, double *cost, double *gradient) const
	{
		const double x = parameters[0];
		const double y = parameters[1];
		
		cost[0] = (1.0 - x) * (1.0 - x) + 100.0 * (y - x * x) * (y - x * x);
		if (gradient != nullptr)
		{
			gradient[0] = -2.0 * (1.0 - x) - 200.0 * (y - x * x) * 2.0 * x;
			gradient[1] = 200.0 * (y - x * x);
		}
		return true;
	}
	
	virtual int NumParameters () const { return 2; }
};

TEST(CERES_ROSENBROCK, use_gradient_problem_2)
{
	double parameters[2] = {3.0, 5.0};
	
	ceres::GradientProblem problem(new RosenBlock2());
	
	ceres::GradientProblemSolver::Options options;
	options.minimizer_progress_to_stdout = true;
	ceres::GradientProblemSolver::Summary summary;
	ceres::Solve(options, problem, parameters, &summary);
	
	std::cout << summary.BriefReport() << std::endl;
}

/// f(x1,x2,...,xN)=∑(1-x(i))^2+(x(i+1)-x(i)^2)^2

template<int N>
class RosenBlockN : public ceres::FirstOrderFunction
{
public:
	virtual bool Evaluate (const double *para, double *cost, double *gradient) const
	{
		cost[0] = 0;
		for (int i = 0; i < N - 1; i++)
		{
			cost[0] = cost[0] + (1 - para[i] * para[i]) + 100.0 * (para[1 + i] - para[i] * para[i]) * (para[i + 1] - para[i] * para[i]);
		}
		if (gradient != nullptr)
		{
			for (int i = 0; i < N; i++)
			{
				gradient[i] = 0;
			}
			for (int i = 0; i < N - 1; i++)
			{
				gradient[i] = -2.0 * (1.0 - para[i]) + 100.0 * (para[i + 1] - para[i] * para[i]) * (para[i + 1] - para[i] * para[i]);
			}
			for (int i = 1; i < N; i++)
			{
				gradient[i] = gradient[i] + 2.0 * 100.0 * (para[i] - para[i - 1] * para[i - 1]);
			}
		}
		return true;
	}
	
	virtual int NumParameters () const { return N; }
};

TEST(CERES_ROSENBROCK, use_gradient_problem_n)
{
	double parameters[5] = {1.0, 1.0, 4.0, -2.0, 1.0};
	
	ceres::GradientProblem problem(new RosenBlockN<5>());
	
	ceres::GradientProblemSolver::Options options;
	options.minimizer_progress_to_stdout = true;
	ceres::GradientProblemSolver::Summary summary;
	ceres::Solve(options, problem, parameters, &summary);
	
	std::cout << summary.BriefReport() << std::endl;
}