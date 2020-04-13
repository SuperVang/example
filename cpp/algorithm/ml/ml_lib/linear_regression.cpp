//
// Created by books on 19-11-17.
//

#include "linear_regression.h"

LinearRegression::LinearRegression()
{

}

LinearRegression::~LinearRegression()
{

}

void LinearRegression::gradientDecent(const SampleVec &pos, const SampleVec &neg)
{
    assert(pos.size() + neg.size() > 1);
    const int N = pos.empty() ? neg.front().rows() : pos.front().rows();

    m_theta = Eigen::VectorXd::Ones(N);

    double cost = calCost(pos, neg);
    bool no_change = false;
    double alpha = 0.03;

    int num = 0;
    while (!no_change && num++ < 3000)
    {

        Eigen::VectorXd delta_theta = Eigen::VectorXd::Zero(N);
        for (size_t i = 0; i < pos.size(); i++)
        {
            delta_theta.noalias() += pos[i] * (pos[i].transpose() * m_theta - 1);
        }
        for (size_t i = 0; i < neg.size(); i++)
        {
            delta_theta.noalias() += neg[i] * (neg[i].transpose() * m_theta);
        }
        delta_theta /= (double) (pos.size() + neg.size());

//		std::cout <<"Theta = "<< m_theta.transpose() << std::endl;
//		std::cout<<"Delta Theta = "<<delta_theta.transpose()<<std::endl;

        m_theta = m_theta - alpha * delta_theta;

        double update_cost = calCost(pos, neg);
        no_change = std::abs(update_cost - cost) < 0.001;
        cost = update_cost;
    }
    std::cout << "Linear Regression Gradient Descent : " << m_theta.transpose() << std::endl;
}

int LinearRegression::predict(const Sample &sample)
{
    return sample.transpose() * m_theta > 0.5 ? 1 : 0;
}

double LinearRegression::calCost(const SampleVec &pos, const SampleVec &neg)
{
    double cost = 0;
    for (size_t i = 0; i < pos.size(); i++)
    {
        cost += std::pow(1 - pos[i].transpose() * m_theta, 2);
    }
    for (size_t i = 0; i < neg.size(); i++)
    {
        cost += std::pow(-neg[i].transpose() * m_theta, 2);
    }
    return cost / (2.0 * (pos.size() + neg.size()));
}

void LinearRegression::normalEquation(const SampleVec &pos, const SampleVec &neg)
{
    assert(pos.size() + neg.size() > 1);
    const int M = pos.size() + neg.size();
    const int N = pos.empty() ? neg.front().rows() : pos.front().rows();

    Eigen::MatrixXd X = Eigen::MatrixXd::Zero(M, N);
    Eigen::VectorXd labels = Eigen::VectorXd::Zero(M);
    for (size_t i = 0; i < pos.size(); i++)
    {
        X.row(i) = pos[i].transpose();
        labels(i) = 1;
    }
    for (size_t i = 0; i < neg.size(); i++)
    {
        X.row(i + pos.size()) = neg[i].transpose();
        labels(i + pos.size()) = 0;
    }

    Eigen::MatrixXd XtX = X.transpose() * X;
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(XtX, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::VectorXd singular_vals = svd.singularValues();
    Eigen::MatrixXd singular_inv = Eigen::MatrixXd::Zero(N, N);
    for (int i = 0; i < N; i++)
    {
        if (singular_vals(i) > 1e-8)
            singular_inv(i, i) = 1.0 / singular_vals(i);
    }
    Eigen::MatrixXd pesudo_inv = svd.computeV() * singular_inv * svd.matrixU().adjoint();

    m_theta = -pesudo_inv * X.transpose() * labels;
    std::cout << "Linear Regression Normal Equation : " << m_theta.transpose() << std::endl;
}

void LinearRegression::gradientDecentWithRegularization(const SampleVec &pos, const SampleVec &neg)
{
    assert(pos.size() + neg.size() > 1);
    const int N = pos.empty() ? neg.front().rows() : pos.front().rows();

    m_theta = Eigen::VectorXd::Ones(N);

    double cost = calCostWithRegularization(pos, neg);
    bool no_change = false;
    double alpha = 0.03;

    Eigen::VectorXd lambda = Eigen::VectorXd::Constant(N, 100.0);
    lambda(0) = 0.0;

    int num = 0;
    while (!no_change && num++ < 3000)
    {

        Eigen::VectorXd delta_theta = Eigen::VectorXd::Zero(N);
        for (size_t i = 0; i < pos.size(); i++)
        {
            delta_theta += (pos[i] * (pos[i].transpose() * m_theta - 1) + lambda.cwiseProduct(m_theta));
        }
        for (size_t i = 0; i < neg.size(); i++)
        {
            delta_theta += (neg[i] * (neg[i].transpose() * m_theta) + lambda.cwiseProduct(m_theta));
        }
        delta_theta /= (double) (pos.size() + neg.size());

//		std::cout <<"Theta = "<< m_theta.transpose() << std::endl;
//		std::cout<<"Delta Theta = "<<delta_theta.transpose()<<std::endl;

        m_theta = m_theta - alpha * delta_theta;

        double update_cost = calCostWithRegularization(pos, neg);
        no_change = std::abs(update_cost - cost) < 0.01;
        cost = update_cost;
    }
    std::cout << "Linear Regression Gradient Descent With Regularization : " << m_theta.transpose() << std::endl;
}

void LinearRegression::normalEquationWithRegularization(const SampleVec &pos, const SampleVec &neg)
{
    assert(pos.size() + neg.size() > 1);
    const int M = pos.size() + neg.size();
    const int N = pos.empty() ? neg.front().rows() : pos.front().rows();

    Eigen::MatrixXd X = Eigen::MatrixXd::Zero(M, N);
    Eigen::VectorXd labels = Eigen::VectorXd::Zero(M);
    for (size_t i = 0; i < pos.size(); i++)
    {
        X.row(i) = pos[i].transpose();
        labels(i) = 1;
    }
    for (size_t i = 0; i < neg.size(); i++)
    {
        X.row(i + pos.size()) = neg[i].transpose();
        labels(i + pos.size()) = 0;
    }

    Eigen::VectorXd lambda = Eigen::VectorXd::Constant(N, 100.0);
    lambda(0) = 0.0;
    Eigen::MatrixXd lambda_mat = lambda.asDiagonal();

    Eigen::MatrixXd XtX = X.transpose() * X;
    XtX = XtX + lambda_mat;
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(XtX, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::VectorXd singular_vals = svd.singularValues();
    Eigen::MatrixXd singular_inv = Eigen::MatrixXd::Zero(N, N);
    for (int i = 0; i < N; i++)
    {
        if (singular_vals(i) > 1e-8)
            singular_inv(i, i) = 1.0 / singular_vals(i);
    }
    Eigen::MatrixXd pesudo_inv = svd.computeV() * singular_inv * svd.matrixU().adjoint();

    m_theta = -pesudo_inv * X.transpose() * labels;
    std::cout << "Linear Regression Normal Equation With Regularization : " << m_theta.transpose() << std::endl;
}

double LinearRegression::calCostWithRegularization(const SampleVec &pos, const SampleVec &neg)
{
    double cost = 0;
    assert(pos.size() + neg.size() > 1);
    const int N = pos.empty() ? neg.front().rows() : pos.front().rows();
    Eigen::VectorXd lambda = Eigen::VectorXd::Constant(N, 100.0);
    lambda(0) = 0.0;
    for (int i = 0; i < m_theta.rows(); i++)
    {
        cost += lambda(i) * m_theta(i) * m_theta(i);
    }
    for (size_t i = 0; i < pos.size(); i++)
    {
        cost += std::pow(1 - pos[i].transpose() * m_theta, 2);
    }
    for (size_t i = 0; i < neg.size(); i++)
    {
        cost += std::pow(-neg[i].transpose() * m_theta, 2);
    }
    return cost / (2.0 * ((int)pos.size() + (int)neg.size()));
}
