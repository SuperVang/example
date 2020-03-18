//
// Created by books on 19-10-14.
//

#ifndef EXAMPLE_MVG_DEF_H
#define EXAMPLE_MVG_DEF_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <random>
#include <ctime>

namespace mvg
{
	typedef Eigen::Matrix3d Transform2D;
	typedef Eigen::Matrix4d Transform3D;
	
	typedef Eigen::Vector3d Pt2D;
	typedef std::vector<Pt2D, Eigen::aligned_allocator<Pt2D>> Pt2DVec;
	
	typedef Eigen::Vector4d Pt3D;
	typedef std::vector<Pt3D, Eigen::aligned_allocator<Pt3D>> Pt3DVec;
	
	typedef Eigen::Vector3d Line2D;
	
	typedef Eigen::Matrix<double, 6, 1> QuadraticCurveLine2D;
	
	typedef Eigen::Matrix<double,6,1> Line3D;
	typedef Eigen::Vector4d Surface3D;

	typedef Eigen::Matrix4d QuadraticCurveSurface3D;
	
	
	bool isProjective2D (const Transform2D &t);
	
	Transform2D buildTransform2D (const Eigen::Matrix<double, 9, 1> &v);
	
	Transform2D buildTransform2D (double theta, double dx, double dy, double s = 1.0);

	Transform3D buildTransform3D(const Eigen::Matrix<double,16,1> &v);

	Transform3D buildTransform3D(double roll,double pitch,double yaw,double dx,double dy,double dz,double s=1.0);

	Eigen::Matrix<double, 9, 1> verticalTransform2D (const Transform2D &T);
	
	bool isInfinitePoint (const Pt2D &pt);

	bool isInfinitePoint(const Pt3D &pt);
	
	bool pointOnLine (const Pt2D &pt, const Line2D &line);
	
	bool isParallelLines (const Line2D &line1, const Line2D &line2);
	
	bool isInfiniteLine (const Line2D &line);
	
	Pt2D getLinesCrossPt (const Line2D &line1, const Line2D &line2);
	
	Line2D buildLineFromPts (const Pt2D &pt1, const Pt2D &pt2);
	
	Line2D transform (const Transform2D &H, const Line2D &line);
	
	QuadraticCurveLine2D buildQuadraticCurveLine2D(double a, double b, double c, double d, double e, double f);
	
	bool pointOnQuadraticCurveLine(const Pt2D &pt, const QuadraticCurveLine2D &curve);
	
	Line2D quadraticCurveTangent (const QuadraticCurveLine2D &curve, const Pt2D &pt);
	
	QuadraticCurveLine2D transform (const Transform2D &H, const QuadraticCurveLine2D &curve);
	
	Line3D buildLineFromPts (const Pt3D &pt1, const Pt3D &pt2);

	Eigen::Matrix4d pluckerMatrix(const Line3D &line);
    Line3D pluckerVector(const Eigen::Matrix4d &mat);
	
	bool pointOnLine(const Pt3D &pt,const Line3D &line);

	Eigen::Vector4d lineDirection(const Line3D &line);

	Line3D transform(const Transform3D &H,const Line3D &line);

	bool lineOnSurface(const Line3D &line,const Surface3D &surface);

	Pt3D lineIntersectSurface(const Line3D &line,const Surface3D &surface);

	bool pointOnSurface(const Surface3D &surface,const Pt3D &pt);
	
	bool isInfinite(const Surface3D &surface);
	
	Surface3D buildSurface(const Pt3D &pt1,const Pt3D &pt2,const Pt3D &pt3);

	Surface3D buildSurface(const Pt3D &pt,const Line3D &line);

	Surface3D transform(const Transform3D &H,const Surface3D &surface);



}

#endif //EXAMPLE_MVG_DEF_H
