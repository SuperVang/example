//
// Created by books on 19-10-13.
//

#include "algeba.h"

namespace mvg
{
	namespace algeba
	{
		
		Eigen::Matrix<double, 2, 9> linearAProjective2D (const Pt2D &pt1, const Pt2D &pt2)
		{
			Eigen::Matrix<double, 2, 9> A;
			A.block(0, 0, 1, 3) = Eigen::RowVector3d::Zero();
			A.block(0, 3, 1, 3) = -pt2[2] * pt1.transpose();
			A.block(0, 6, 1, 3) = pt2[1] * pt1.transpose();
			A.block(1, 0, 1, 3) = pt2[2] * pt1.transpose();
			A.block(1, 3, 1, 3) = Eigen::RowVector3d::Zero();
			A.block(1, 6, 1, 3) = -pt2[0] * pt1.transpose();
			return A;
		}
		
		Eigen::Matrix<double, 2, 4> geometryJacobianProjective2D (const Transform2D &T, const Pt2D &pt1, const Pt2D &pt2)
		{
			Eigen::Matrix<double, 9, 1> v = verticalTransform2D(T);
			Eigen::Matrix<double, 2, 4> j;
			double x1 = pt1(0), y1 = pt1(1), w1 = pt1(2);
			double x2 = pt2(0), y2 = pt2(1), w2 = pt2(2);
			j(0, 0) = -w2 * v(3) + y2 * v(6);
			j(0, 1) = -w2 * v(4) + x1 * v(6) + y2 * v(7);
			j(0, 2) = 0;
			j(0, 3) = x1 * v(6) + y1 * v(7) + w1 * v(8);
			j(1, 0) = w2 * v(0) - x2 * v(6);
			j(1, 1) = w2 * v(1) - x2 * v(7);
			j(1, 2) = -x1 * v(6) - y1 * v(7) - w1 * v(8);
			j(1, 3) = 0;
			return j;
		}
		
		double transferResidual (const Pt2D &pt1, const Pt2D &pt2, const Transform2D &T12)
		{
			Pt2D pt12 = T12 * pt1;
			double dis1 = std::hypot(pt12[0] - pt2[0], pt12[1] - pt2[1]);
			
			Pt2D pt21 = T12.inverse() * pt2;
			double dis2 = std::hypot(pt1[0] - pt21[0], pt1[1] - pt21[1]);
			
			return dis1 + dis2;
		}
		
		double transferResidual (const Pt2DVec &pts1, const Pt2DVec &pts2, const Transform2D &T12)
		{
			double dis = 0;
			for (size_t i = 0; i < pts1.size(); i++)
			{
				dis += transferResidual(pts1[i], pts2[i], T12);
			}
			return dis / pts1.size();
		}
		
		double sampsonResidual (const Pt2D &pt1, const Pt2D &pt2, const Transform2D &T12)
		{
			Eigen::Vector2d ktheta = linearAProjective2D(pt1, pt2) * verticalTransform2D(T12);
			Eigen::Matrix<double, 2, 4> J = geometryJacobianProjective2D(T12, pt1, pt2);
			return std::abs(ktheta.transpose() * (J * J.transpose()).inverse() * ktheta);
		}
		
		double sampsonResidual (const Pt2DVec &pts1, const Pt2DVec &pts2, const Transform2D &T12)
		{
			const int n = pts1.size();
			double sum = 0;
			for (int i = 0; i < n; i++)
			{
				sum += sampsonResidual(pts1[i], pts2[i], T12);
			}
			return sum / n;
		}
		
		
	}
}