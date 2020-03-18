//
// Created by books on 19-11-17.
//

#include <iostream>
#include <Eigen/Core>
#include <gtest/gtest.h>
#include <random>

#include "ml_lib/logistic_regression.h"


TEST(ML_LOGISTIC_REGRESSION, gradient_descent)
{
	static std::default_random_engine e(std::time(nullptr));
	std::normal_distribution<double> normalDis(0, 1.2);
	
	SampleVec pos, neg;
	for (int i = 0; i < 30; i++)
	{
		double x = normalDis(e);
		double y = normalDis(e);
		Sample tmp_sample(5);
		tmp_sample << 1, x, y, x * x, y * y;
		if (x * x + y * y < 1.0)
		{
			pos.push_back(tmp_sample);
		}
		else
		{
			neg.push_back(tmp_sample);
		}
	}
	
	LogisticRegression lr;
    lr.gradientDescent(pos, neg);
	
	Sample p1(5);
	p1 << 1, 0.5, 0, 0.25, 0;
	EXPECT_EQ(1, lr.predict(p1));
	
	Sample p2(5);
	p2 << 1, 2, 0, 4, 0;
	EXPECT_EQ(0, lr.predict(p2));
}

TEST(ML_LOGISTIC_REGRESSION,gradient_descent_with_regularization)
{
    static std::default_random_engine e(std::time(nullptr));
    std::normal_distribution<double> normalDis(0, 1.2);

    SampleVec pos, neg;
    for (int i = 0; i < 30; i++)
    {
        double x = normalDis(e);
        double y = normalDis(e);
        Sample tmp_sample(5);
        tmp_sample << 1, x, y, x * x, y * y;
        if (x * x + y * y < 1.0)
        {
            pos.push_back(tmp_sample);
        }
        else
        {
            neg.push_back(tmp_sample);
        }
    }

    LogisticRegression lr;
    lr.gradientDescentRegularization(pos, neg);

    Sample p1(5);
    p1 << 1, 0.5, 0, 0.25, 0;
    EXPECT_EQ(1, lr.predict(p1));

    Sample p2(5);
    p2 << 1, 2, 0, 4, 0;
    EXPECT_EQ(0, lr.predict(p2));
}