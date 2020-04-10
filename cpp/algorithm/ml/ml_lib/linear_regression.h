//
// Created by books on 19-11-17.
//

#ifndef EXAMPLE_LINEAR_REGRESSION_H
#define EXAMPLE_LINEAR_REGRESSION_H

#include <iostream>
#include <Eigen/Core>
#include <Eigen/SVD>

#include "ml_def.h"

/**
 * @brief 
 * 
 */
class LinearRegression
{
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
	LinearRegression ();
	
	~LinearRegression ();

	/// use gradient descent
	/// \param pos
	/// \param neg
	void gradientDecent(const SampleVec &pos, const SampleVec &neg);

	/// use normal equation
	/// \param pos
	/// \param neg
	void normalEquation(const SampleVec &pos, const SampleVec &neg);

	void gradientDecentWithRegularization(const SampleVec &pos,const SampleVec &neg);

	void normalEquationWithRegularization(const SampleVec &pos,const SampleVec &neg);
	
	int predict(const Sample &sample);
	
private:
	
	double calCost(const SampleVec &pos,const SampleVec &neg);

	double calCostWithRegularization(const SampleVec &pos,const SampleVec &neg);
	
	Eigen::VectorXd m_theta;
	
	
};


#endif //EXAMPLE_LINEAR_REGRESSION_H
