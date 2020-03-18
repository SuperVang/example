//
// Created by books on 19-10-14.
//

#include <iostream>
#include <Eigen/Core>
#include <gtest/gtest.h>

#include "mvglib/mvg_def.h"

TEST(MVG_DEF, point_on_line_2d)
{
	mvg::Pt2D pt(2, 2, 1);
	
	mvg::Line2D line(1, -1, 0);
	
	EXPECT_TRUE(mvg::pointOnLine(pt, line));
}

TEST(MVG_DEF, paraller_lines_2d)
{
	mvg::Line2D line1(1, 1, 2);
	mvg::Line2D line2(3, 3, 0);
	EXPECT_TRUE(mvg::isParallelLines(line1, line2));
}

TEST(MVG_DEF, build_line_from_two_pt_2d)
{
	mvg::Pt2D pt1(1, 1, 1);
	mvg::Pt2D pt2(2, 2, 1);
	
	mvg::Line2D line = mvg::buildLineFromPts(pt1, pt2);
	EXPECT_EQ(line(0), -line(1));
	EXPECT_EQ(line(2), 0);
	
}

TEST(MVG_DEF, get_line_cross_pt_2d)
{
	mvg::Line2D line1(1, 1, -2);
	mvg::Line2D line2(1, -1, 0);
	mvg::Pt2D cross = mvg::getLinesCrossPt(line1, line2);
	EXPECT_EQ(cross(0), cross(1));
	EXPECT_EQ(cross(0), 1);
}

TEST(MVG_DEF, point_on_curve_2d)
{
	mvg::QuadraticCurveLine2D circle = mvg::buildQuadraticCurveLine2D(1, 0, 1, -2, -2, 1);
	mvg::Pt2D pt(1, 0, 1);
	EXPECT_TRUE(mvg::pointOnQuadraticCurveLine(pt, circle));
}

TEST(MVG_DEF, build_line_from_two_pt_3d)
{
	mvg::Pt3D pt1(0, 0, 0, 1);
	mvg::Pt3D pt2(5, 5, 5, 1);
	mvg::Line3D line= mvg::buildLineFromPts(pt1, pt2);
	std::cout<<line<<std::endl;
}

TEST(MVG_DEF,point_on_line_3d)
{
	mvg::Pt3D pt1(0, 0, 0, 1);
	mvg::Pt3D pt2(5, 5, 5, 1);
	mvg::Line3D line= mvg::buildLineFromPts(pt1, pt2);
	mvg::Pt3D pt(3,3,3,1);
	EXPECT_TRUE(mvg::pointOnLine(pt,line));
}

TEST(MVG_DEF, build_transform_from_vertical_vector)
{
	Eigen::Matrix<double, 9, 1> v;
	v << 1, 0, 0, 0, 1, 0, 0, 0, 1;
	mvg::Transform2D t = mvg::buildTransform2D(v);
	
	EXPECT_TRUE(mvg::isProjective2D(t));
}