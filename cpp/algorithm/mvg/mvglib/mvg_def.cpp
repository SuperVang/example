//
// Created by books on 19-10-14.
//

#include "mvg_def.h"

namespace mvg
{
	
	bool isProjective2D (const Transform2D &t)
	{
		return t.determinant() != 0;
	}
	
	Transform2D buildTransform2D (const Eigen::Matrix<double, 9, 1> &v)
	{
		Transform2D t;
		t.block(0, 0, 1, 3) = v.block(0, 0, 3, 1).transpose();
		t.block(1, 0, 1, 3) = v.block(3, 0, 3, 1).transpose();
		t.block(2, 0, 1, 3) = v.block(6, 0, 3, 1).transpose();
		assert(isProjective2D(t));
		return t;
		
	}
	
	Transform2D buildTransform2D (double theta, double dx, double dy, double s)
	{
		Transform2D t = Transform2D::Identity();
		t(0, 0) = s * std::cos(theta);
		t(0, 1) = -s * std::sin(theta);
		t(0, 2) = dx;
		t(1, 0) = s * std::sin(theta);
		t(1, 1) = s * std::cos(theta);
		t(1, 2) = dy;
		assert(isProjective2D(t));
		return t;
	}
	
	Transform3D buildTransform3D (const Eigen::Matrix<double, 16, 1> &v)
	{
		Transform3D T;
		T.block(0, 0, 1, 4) = v.block(0, 0, 4, 1).transpose();
		T.block(1, 0, 1, 4) = v.block(4, 0, 4, 1).transpose();
		T.block(2, 0, 1, 4) = v.block(8, 0, 4, 1).transpose();
		T.block(3, 0, 1, 4) = v.block(12, 0, 4, 1).transpose();
		return T;
	}
	
	Transform3D buildTransform3D (double roll, double pitch, double yaw, double dx, double dy, double dz, double s)
	{
		Transform3D T = Transform3D::Identity();
		
		Eigen::AngleAxisd rollAngle(roll, Eigen::Vector3d::UnitX());
		Eigen::AngleAxisd pitchAngle(pitch, Eigen::Vector3d::UnitY());
		Eigen::AngleAxisd yawAngle(yaw, Eigen::Vector3d::UnitZ());
		Eigen::Matrix3d rotationMat;
		rotationMat = yawAngle * pitchAngle * rollAngle;
		
		T.block(0, 0, 3, 3) = s * rotationMat;
		T.block(0, 3, 3, 1) = Eigen::Vector3d(dx, dy, dz);
		
		return T;
	}
	
	
	Eigen::Matrix<double, 9, 1> verticalTransform2D (const Transform2D &T)
	{
		Eigen::Matrix<double, 9, 1> v;
		v.block(0, 0, 3, 1) = T.block(0, 0, 1, 3).transpose();
		v.block(3, 0, 3, 1) = T.block(1, 0, 1, 3).transpose();
		v.block(6, 0, 3, 1) = T.block(2, 0, 1, 3).transpose();
		return v;
	}
	
	bool pointOnLine (const Pt2D &pt, const Line2D &line)
	{
		return pt.transpose() * line == 0;
	}
	
	Pt2D getLinesCrossPt (const Line2D &line1, const Line2D &line2)
	{
		Eigen::Vector3d cross = line1.cross(line2);
		if (cross(2) == 0)
			return cross;
		return cross / cross(2);
	}
	
	bool isInfinitePoint (const Pt2D &pt)
	{
		return pt(2) == 0;
	}
	
	bool isInfinitePoint (const Pt3D &pt)
	{
		return pt(3) == 0;
	}
	
	
	Line2D buildLineFromPts (const Pt2D &pt1, const Pt2D &pt2)
	{
		return pt1.cross(pt2);
	}
	
	bool isParallelLines (const Line2D &line1, const Line2D &line2)
	{
		Eigen::Vector3d cross = line1.cross(line2);
		return cross(2) == 0;
	}
	
	bool isInfiniteLine (const Line2D &line)
	{
		return line(0) == 0 && line(1) == 0;
	}
	
	bool pointOnQuadraticCurveLine (const Pt2D &pt, const QuadraticCurveLine2D &curve)
	{
		Eigen::Matrix3d C;
		C << curve(0), curve(1) / 2.0, curve(3) / 2.0, curve(1) / 2.0, curve(2), curve(4) / 2.0, curve(3) / 2.0,
				curve(4) / 2.0, curve(5);
		
		return pt.transpose() * C * pt == 0;
	}
	
	QuadraticCurveLine2D buildQuadraticCurveLine2D (double a, double b, double c, double d, double e, double f)
	{
		QuadraticCurveLine2D curve;
		curve << a, b, c, d, e, f;
		return curve;
	}
	
	Line2D quadraticCurveTangent (const QuadraticCurveLine2D &curve, const Pt2D &pt)
	{
		Eigen::Matrix3d C;
		C << curve(0), curve(1) / 2.0, curve(3) / 2.0, curve(1) / 2.0, curve(2), curve(4) / 2.0, curve(3) / 2.0,
				curve(4) / 2.0, curve(5);
		return C * pt;
	}
	
	Line2D transform (const Transform2D &H, const Line2D &line)
	{
		return H.transpose().inverse() * line;
	}
	
	QuadraticCurveLine2D transform (const Transform2D &H, const QuadraticCurveLine2D &curve)
	{
		Eigen::Matrix3d C;
		C << curve(0), curve(1) / 2.0, curve(3) / 2.0, curve(1) / 2.0, curve(2), curve(4) / 2.0, curve(3) / 2.0,
				curve(4) / 2.0, curve(5);
		Eigen::Matrix3d CH = H.transpose().inverse() * C * H.inverse();
		double a = CH(0, 0);
		double b = CH(0, 1) * 2.0;
		double c = CH(1, 1);
		double d = CH(0, 2) * 2.0;
		double e = CH(1, 2) * 2.0;
		double f = CH(2, 2);
		return buildQuadraticCurveLine2D(a, b, c, d, e, f);
	}
	
	bool pointOnSurface (const Surface3D &surface, const Pt3D &pt)
	{
		return surface.transpose() * pt == 0;
	}
	
	Surface3D buildSurface (const Pt3D &pt1, const Pt3D &pt2, const Pt3D &pt3)
	{
		Eigen::Matrix<double, 4, 3> pts;
		pts.col(0) = pt1;
		pts.col(1) = pt2;
		pts.col(2) = pt3;
		
		Eigen::Matrix3d D234, D134, D124, D123;
		D234.block(0, 0, 3, 3) = pts.block(1, 0, 3, 3);
		D134.row(0) = pts.row(0);
		D134.block(1, 0, 2, 3) = pts.block(2, 0, 2, 3);
		D124.block(0, 0, 2, 3) = pts.block(0, 0, 2, 3);
		D124.row(2) = pts.row(3);
		D123.block(0, 0, 3, 3) = pts.block(0, 0, 3, 3);
		
		Surface3D surface;
		surface << D234.determinant(), -D134.determinant(), D124.determinant(), -D123.determinant();
		return surface;
		
	}
	
	Line3D buildLineFromPts (const Pt3D &pt1, const Pt3D &pt2)
	{
		Eigen::Matrix4d mat = pt1 * pt2.transpose() - pt2 * pt1.transpose();
		return pluckerVector(mat);
	}
	
	bool pointOnLine (const Pt3D &pt, const Line3D &line)
	{
		Line3D dual;
		dual << line(5), -line(4), line(3), line(2), -line(1), line(0);
		Eigen::Matrix4d dualMat = pluckerMatrix(dual);
		return dualMat * pt == Surface3D::Zero();
	}
	
	Eigen::Matrix4d pluckerMatrix (const Line3D &line)
	{
		Eigen::Matrix4d mat = Eigen::Matrix4d::Zero();
		mat(0, 1) = -line(0);
		mat(0, 2) = -line(1);
		mat(0, 3) = -line(2);
		mat(1, 2) = -line(3);
		mat(1, 3) = -line(4);
		mat(2, 3) = -line(5);
		mat(1, 0) = line(0);
		mat(2, 0) = line(1);
		mat(3, 0) = line(2);
		mat(2, 1) = line(3);
		mat(3, 1) = line(4);
		mat(3, 2) = line(5);
		return mat;
	}
	
	Line3D pluckerVector (const Eigen::Matrix4d &mat)
	{
		double t01 = -mat(0, 1);
		double t02 = -mat(0, 2);
		double t03 = -mat(0, 3);
		double t12 = -mat(1, 2);
		double t13 = -mat(1, 3);
		double t23 = -mat(2, 3);
		
		Line3D line;
		line << t01, t02, t03, t12, t13, t23;
		return line;
	}
	
	bool lineOnSurface (const Line3D &line, const Surface3D &surface)
	{
		Eigen::Matrix4d mat = pluckerMatrix(line);
		return mat * surface == Pt3D::Zero();
	}
	
	Pt3D lineIntersectSurface (const Line3D &line, const Surface3D &surface)
	{
		Eigen::Matrix4d mat = pluckerMatrix(line);
		return mat * surface;
	}
	
	Surface3D buildSurface (const Pt3D &pt, const Line3D &line)
	{
		Line3D dual;
		dual << line(5), -line(4), line(3), line(2), -line(1), line(0);
		Eigen::Matrix4d dualMat = pluckerMatrix(dual);
		return dualMat * pt;
	}
	
	Eigen::Vector4d lineDirection (const Line3D &line)
	{
		Eigen::Matrix4d mat = pluckerMatrix(line);
		Surface3D inf;
		inf << 0, 0, 0, 1;
		return mat * inf;
	}
	
	
	Line3D transform (const Transform3D &H, const Line3D &line)
	{
		Eigen::Matrix4d pl = pluckerMatrix(line);
		Eigen::Matrix4d pl_trans = H * pl * H.transpose();
		return pluckerVector(pl_trans);
	}
	
	Surface3D transform (const Transform3D &H, const Surface3D &surface)
	{
		return H.inverse().transpose() * surface;
	}
	
	bool isInfinite (const Surface3D &surface)
	{
		return surface(0) == 0 && surface(1) == 0 && surface(2) == 0 && surface(3) != 0;
		
	}
	
	
}