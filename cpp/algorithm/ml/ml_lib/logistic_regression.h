//
// Created by books on 19-11-17.
//

#ifndef EXAMPLE_LOGISTIC_REGRESSION_H
#define EXAMPLE_LOGISTIC_REGRESSION_H


#include <iostream>
#include <Eigen/Core>

#include "ml_def.h"

class LogisticRegression
{
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
	LogisticRegression ();
	
	~LogisticRegression ();
	
	void gradientDescent(const SampleVec &pos, const SampleVec &neg);

	void gradientDescentRegularization(const SampleVec &pos, const SampleVec &neg);
	
	int predict(const Sample &sample);

private:
	
	double logistic(const Sample &sample);
	
	double calCost(const SampleVec &pos,const SampleVec &neg);

	double calCostRegularization(const SampleVec &pos,const SampleVec &neg);
	
	Eigen::VectorXd m_theta;
};


#endif //EXAMPLE_LOGISTIC_REGRESSION_H
