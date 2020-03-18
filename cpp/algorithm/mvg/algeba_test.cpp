//
// Created by books on 19-10-13.
//

#include <iostream>
#include <gtest/gtest.h>

#include "mvglib/mvg_util.h"
#include "mvglib/algeba.h"

TEST(MVG_ALGEBA, transfer_residual)
{
	const int n = 10;
	double noise = 0.5;
	
	mvg::Pt2DVec pts1 = mvg::util::buildRandomPts2D(n);
	mvg::Pt2DVec pts2 = mvg::util::buildRandomPts2D(n);
	mvg::Pt2DVec noisePts1 = mvg::util::buildRandomPts2D(pts1, noise);
	mvg::Pt2DVec noisePts2 = mvg::util::buildRandomPts2D(pts2, noise);
	
	double dis1 = mvg::algeba::transferResidual(pts1, noisePts1);
	double dis2 = mvg::algeba::transferResidual(pts2, noisePts2);
	EXPECT_NE(dis1, dis2);
}

TEST(MVG_ALGEBA,sampson_residual)
{
	const int n = 10;
	double noise = 0.5;
	
	mvg::Pt2DVec pts1 = mvg::util::buildRandomPts2D(n);
	mvg::Pt2DVec pts2 = mvg::util::buildRandomPts2D(n);
	mvg::Pt2DVec noisePts1 = mvg::util::buildRandomPts2D(pts1, noise);
	mvg::Pt2DVec noisePts2 = mvg::util::buildRandomPts2D(pts2, noise);
	
	double dis1 = mvg::algeba::sampsonResidual(pts1, noisePts1);
	double dis2 = mvg::algeba::sampsonResidual(pts2, noisePts2);
	EXPECT_NE(dis1, dis2);
	
}