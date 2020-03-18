//
// Created by books on 19-11-17.
//

#include "logistic_regression.h"

LogisticRegression::LogisticRegression()
{

}

LogisticRegression::~LogisticRegression()
{

}

void LogisticRegression::gradientDescent(const SampleVec &pos, const SampleVec &neg)
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
            delta_theta.noalias() += pos[i] * (logistic(pos[i]) - 1);
        }
        for (size_t i = 0; i < neg.size(); i++)
        {
            delta_theta.noalias() += neg[i] * (logistic(neg[i]));
        }
        delta_theta /= (double) (pos.size() + neg.size());

        m_theta = m_theta - alpha * delta_theta;

        double update_cost = calCost(pos, neg);
        no_change = std::abs(update_cost - cost) < 0.01;
        cost = update_cost;
    }
    std::cout << "Logistic Regression Gradient Descent : " << m_theta.transpose() << std::endl;
}

void LogisticRegression::gradientDescentRegularization(const SampleVec &pos, const SampleVec &neg)
{
    assert(pos.size() + neg.size() > 1);
    const int N = pos.empty() ? neg.front().rows() : pos.front().rows();

    m_theta = Eigen::VectorXd::Ones(N);

    double cost = calCostRegularization(pos, neg);
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
            delta_theta.noalias() += (pos[i] * (logistic(pos[i]) - 1) + lambda.cwiseProduct(m_theta));
        }
        for (size_t i = 0; i < neg.size(); i++)
        {
            delta_theta.noalias() += (neg[i] * (logistic(neg[i])) + lambda.cwiseProduct(m_theta));
        }
        delta_theta /= (double) (pos.size() + neg.size());

        m_theta = m_theta - alpha * delta_theta;

        double update_cost = calCostRegularization(pos, neg);
        no_change = std::abs(update_cost - cost) < 0.01;
        cost = update_cost;
    }
    std::cout << "Logistic Regression Gradient Descent : " << m_theta.transpose() << std::endl;
}

int LogisticRegression::predict(const Sample &sample)
{
    return logistic(sample) > 0.5 ? 1 : 0;
}

double LogisticRegression::calCost(const SampleVec &pos, const SampleVec &neg)
{
    double cost = 0;
    for (size_t i = 0; i < pos.size(); i++)
    {
        cost += std::log(logistic(pos[i]));
    }
    for (size_t i = 0; i < neg.size(); i++)
    {
        cost += std::log(1.0 - logistic(neg[i]));
    }
    return cost / (1.0 * (pos.size() + neg.size()));
}

double LogisticRegression::logistic(const Sample &sample)
{
    return 1.0 / (1.0 + std::exp(-m_theta.transpose() * sample));
}

double LogisticRegression::calCostRegularization(const SampleVec &pos, const SampleVec &neg)
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
        cost += std::log(logistic(pos[i]));
    }
    for (size_t i = 0; i < neg.size(); i++)
    {
        cost += std::log(1.0 - logistic(neg[i]));
    }

    return cost / (1.0 * (pos.size() + neg.size()));
}


