//
// Created by books on 19-10-13.
//

#include <iostream>
#include <random>
#include "gtest/gtest.h"

#include "mvglib/mvg_util.h"
#include "mvglib/ransac.h"
#include "mvglib/algeba.h"

TEST(MVG_RANSAC, random_projective_2d_without_noise)
{
	const int n = 20;
	
	mvg::Transform2D T = mvg::util::buildRandomProjective2D();
	
	mvg::Pt2DVec pts1 = mvg::util::buildRandomPts2D(n);
	mvg::Pt2DVec pts2 = mvg::util::buildRandomPts2D(pts1, T);
	
	mvg::Pt2DVec ransacPts1, ransacPts2;
	ransacPts1.assign(pts1.begin(), pts1.end());
	ransacPts2.assign(pts2.begin(), pts2.end());
	
	mvg::Pt2DVec ransacAdaPts1, ransacAdaPts2;
	ransacAdaPts1.assign(pts1.begin(), pts1.end());
	ransacAdaPts2.assign(pts2.begin(), pts2.end());
	
	mvg::Transform2D H = mvg::ransac::solveProjective2D(ransacPts1, ransacPts2);
	mvg::Transform2D HAda = mvg::ransac::solveProjective2DAdaptive(ransacAdaPts1, ransacAdaPts2);
	mvg::Transform2D HNor = mvg::dlt::solveProjective2DNormalize(pts1, pts2);
	
	std::cout << "Origin = " << mvg::algeba::transferResidual(pts1, pts2, T) << std::endl;
	std::cout << "Ransac = " << mvg::algeba::transferResidual(pts1, pts2, H) << std::endl;
	std::cout << "Ransac Adaptive = " << mvg::algeba::transferResidual(pts1, pts2, HAda) << std::endl;
	std::cout << "DLT Normalize = " << mvg::algeba::transferResidual(pts1, pts2, HNor) << std::endl;
}

TEST(MVG_RANSAC, random_projective_2d_with_noise)
{
	const int n = 20;
	double noise = 0.5;
	
	mvg::Transform2D T = mvg::util::buildRandomProjective2D();
	
	mvg::Pt2DVec pts1 = mvg::util::buildRandomPts2D(n);
	mvg::Pt2DVec pts2 = mvg::util::buildRandomPts2D(pts1, T);
	mvg::Pt2DVec noisePts1 = mvg::util::buildRandomPts2D(pts1, noise);
	mvg::Pt2DVec noisePts2 = mvg::util::buildRandomPts2D(pts2, noise);
	
	mvg::Pt2DVec ransacPts1, ransacPts2;
	ransacPts1.assign(noisePts1.begin(), noisePts1.end());
	ransacPts2.assign(noisePts2.begin(), noisePts2.end());
	
	mvg::Pt2DVec ransacAdaPts1, ransacAdaPts2;
	ransacAdaPts1.assign(noisePts1.begin(), noisePts1.end());
	ransacAdaPts2.assign(noisePts2.begin(), noisePts2.end());
	
	mvg::Transform2D H = mvg::ransac::solveProjective2D(ransacPts1, ransacPts2);
	mvg::Transform2D HAda = mvg::ransac::solveProjective2DAdaptive(ransacAdaPts1, ransacAdaPts2);
	mvg::Transform2D HNor = mvg::dlt::solveProjective2DNormalize(noisePts1, noisePts2);
	
	std::cout << "Origin = " << mvg::algeba::transferResidual(pts1, pts2, T) << std::endl;
	std::cout << "Ransac = " << mvg::algeba::transferResidual(pts1, pts2, H) << std::endl;
	std::cout << "Ransac Adaptive = " << mvg::algeba::transferResidual(pts1, pts2, HAda) << std::endl;
	std::cout << "DLT Normalize= " << mvg::algeba::transferResidual(pts1, pts2, HNor) << std::endl;
}

TEST(MVG_RANSAC, random_projective_2d_with_outlier)
{
	const int n = 20;
	const int m = 5;
	double noise = 0.5;
	
	mvg::Transform2D T = mvg::util::buildRandomProjective2D();
	
	mvg::Pt2DVec pts1 = mvg::util::buildRandomPts2D(n);
	mvg::Pt2DVec pts2 = mvg::util::buildRandomPts2D(pts1, T);
	
	mvg::Pt2DVec pts11 = mvg::util::buildRandomPts2D(m);
	mvg::Pt2DVec pts22 = mvg::util::buildRandomPts2D(pts11, T);
	
	mvg::Pt2DVec outlierPts1, outlierPts2;
	outlierPts1.insert(outlierPts1.end(), pts1.begin(), pts1.end());
	outlierPts1.insert(outlierPts1.end(), pts22.begin(), pts22.end());
	outlierPts2.insert(outlierPts2.end(), pts2.begin(), pts2.end());
	outlierPts2.insert(outlierPts2.end(), pts11.begin(), pts11.end());
	
	mvg::Pt2DVec ransacPts1, ransacPts2;
	ransacPts1.assign(outlierPts1.begin(), outlierPts1.end());
	ransacPts2.assign(outlierPts2.begin(), outlierPts2.end());
	
	mvg::Pt2DVec ransacAdaPts1, ransacAdaPts2;
	ransacAdaPts1.assign(outlierPts1.begin(), outlierPts1.end());
	ransacAdaPts2.assign(outlierPts2.begin(), outlierPts2.end());
	
	mvg::Transform2D H = mvg::ransac::solveProjective2D(ransacPts1, ransacPts2);
	mvg::Transform2D HAda = mvg::ransac::solveProjective2DAdaptive(ransacAdaPts1, ransacAdaPts2);
	mvg::Transform2D HNor = mvg::dlt::solveProjective2DNormalize(outlierPts1, outlierPts2);
	
	std::cout << "Origin = " << mvg::algeba::transferResidual(pts1, pts2, T) << std::endl;
	std::cout << "Ransac = " << mvg::algeba::transferResidual(pts1, pts2, H) << std::endl;
	std::cout << "Ransac Adaptive = " << mvg::algeba::transferResidual(pts1, pts2, HAda) << std::endl;
	std::cout << "DLT Normalize = " << mvg::algeba::transferResidual(pts1, pts2, HNor) << std::endl;
}