
#include <iostream>
#include <random>
#include "gtest/gtest.h"

#include "mvglib/mvg_util.h"
#include "mvglib/dlt.h"
#include "mvglib/algeba.h"

TEST(MVG_DLT, normalize_2d)
{
	const int n = 10;
	
	mvg::Pt2DVec pts = mvg::util::buildRandomPts2D(n);
	
	mvg::Transform2D T = mvg::dlt::normalize(pts);
	mvg::Pt2DVec ptsNorm = mvg::util::buildRandomPts2D(pts, T);
	
	double cenX = 0, cenY = 0;
	for (const mvg::Pt2D &pt : ptsNorm)
	{
		cenX += pt[0];
		cenY += pt[1];
	}
	
	EXPECT_NEAR(0, cenX, 0.001);
	EXPECT_NEAR(0, cenY, 0.001);
	
	double sumDis = 0;
	for (const mvg::Pt2D &pt : ptsNorm)
	{
		sumDis += std::hypot(pt[0] - cenX, pt[1] - cenY);
	}
	EXPECT_NEAR(std::sqrt(2), sumDis / n, 0.001);
}

TEST(MVG_DLT, similar_2d_without_noise)
{
	const int n = 10;
	
	mvg::Transform2D T = mvg::buildTransform2D(45 * M_PI / 180.0, -3, 2, 3);
	
	mvg::Pt2DVec pts1 = mvg::util::buildRandomPts2D(n);
	mvg::Pt2DVec pts2 = mvg::util::buildRandomPts2D(pts1, T);
	
	mvg::Transform2D H = mvg::dlt::solveProjective2D(pts1, pts2);
	mvg::Transform2D HNor = mvg::dlt::solveProjective2DNormalize(pts1, pts2);
	
	std::cout << "Origin = " << mvg::algeba::transferResidual(pts1, pts2, T) << std::endl;
	std::cout << "DLT = " << mvg::algeba::transferResidual(pts1, pts2, H) << std::endl;
	std::cout << "DLT Normalize = " << mvg::algeba::transferResidual(pts1, pts2, HNor) << std::endl;
}

TEST(MVG_DLT, similar_2d_with_noise)
{
	const int n = 10;
	double noise = 1;
	
	mvg::Transform2D T = mvg::buildTransform2D(45 * M_PI / 180.0, -3, 2, 3);
	
	mvg::Pt2DVec pts1 = mvg::util::buildRandomPts2D(n);
	mvg::Pt2DVec pts2 = mvg::util::buildRandomPts2D(pts1, T);
	mvg::Pt2DVec noisePts1 = mvg::util::buildRandomPts2D(pts1, noise);
	mvg::Pt2DVec noisePts2 = mvg::util::buildRandomPts2D(pts2, noise);
	
	mvg::Transform2D H = mvg::dlt::solveProjective2D(noisePts1, noisePts2);
	mvg::Transform2D HNor = mvg::dlt::solveProjective2DNormalize(noisePts1, noisePts2);
	
	std::cout << "Origin = " << mvg::algeba::transferResidual(pts1, pts2, T) << std::endl;
	std::cout << "DLT = " << mvg::algeba::transferResidual(pts1, pts2, H) << std::endl;
	std::cout << "DLT Normalize = " << mvg::algeba::transferResidual(pts1, pts2, HNor) << std::endl;
}

TEST(MVG_DLT, random_projective_2d_without_noise)
{
	const int n = 10;
	
	mvg::Transform2D T = mvg::util::buildRandomProjective2D();
	
	mvg::Pt2DVec pts1 = mvg::util::buildRandomPts2D(n);
	mvg::Pt2DVec pts2 = mvg::util::buildRandomPts2D(pts1, T);
	
	mvg::Transform2D H = mvg::dlt::solveProjective2D(pts1, pts2);
	mvg::Transform2D HNor = mvg::dlt::solveProjective2DNormalize(pts1, pts2);
	
	std::cout << "Origin = " << mvg::algeba::transferResidual(pts1, pts2, T) << std::endl;
	std::cout << "DLT = " << mvg::algeba::transferResidual(pts1, pts2, H) << std::endl;
	std::cout << "DLT Normalize = " << mvg::algeba::transferResidual(pts1, pts2, HNor) << std::endl;
}

TEST(MVG_DLT, random_projective_2d_with_noise)
{
	const int n = 10;
	double noise = 0.5;
	
	mvg::Transform2D T = mvg::util::buildRandomProjective2D();
	
	mvg::Pt2DVec pts1 = mvg::util::buildRandomPts2D(n);
	mvg::Pt2DVec pts2 = mvg::util::buildRandomPts2D(pts1, T);
	mvg::Pt2DVec noisePts1 = mvg::util::buildRandomPts2D(pts1, noise);
	mvg::Pt2DVec noisePts2 = mvg::util::buildRandomPts2D(pts2, noise);
	
	mvg::Transform2D H = mvg::dlt::solveProjective2D(noisePts1, noisePts2);
	mvg::Transform2D HNor = mvg::dlt::solveProjective2DNormalize(noisePts1, noisePts2);
	
	std::cout << "Origin = " << mvg::algeba::transferResidual(pts1, pts2, T) << std::endl;
	std::cout << "DLT = " << mvg::algeba::transferResidual(pts1, pts2, H) << std::endl;
	std::cout << "DLT Normalize = " << mvg::algeba::transferResidual(pts1, pts2, HNor) << std::endl;
}
