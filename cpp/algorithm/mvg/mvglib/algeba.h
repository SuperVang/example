//
// Created by books on 19-10-13.
//

#ifndef EXAMPLE_ALGEBA_H
#define EXAMPLE_ALGEBA_H

#include <iostream>
#include <Eigen/Core>

#include "mvg_def.h"

namespace mvg
{
	namespace algeba
	{
		
		/// A*h=0 -> A
		/// the matrix when calculate
		/// \param pt1
		/// \param pt2
		/// \return
		Eigen::Matrix<double, 2, 9> linearAProjective2D (const Pt2D &pt1, const Pt2D &pt2);
		
		/// 计算仿射变换T相对于变换点对(pt1,pt2)的Jacobian矩阵
		/// \param T
		/// \param pt1
		/// \param pt2
		/// \return
		Eigen::Matrix<double, 2, 4> geometryJacobianProjective2D (const Transform2D &T, const Pt2D &pt1, const Pt2D &pt2);
		
		/// transfer residual of one pair point under transform
		/// \param pt1
		/// \param pt2
		/// \param T12
		/// \return
		double transferResidual (const Pt2D &pt1, const Pt2D &pt2, const Transform2D &T12 = Transform2D::Identity());
		
		/// transfer residual of some pairs point under transform
		/// \param pts1
		/// \param pts2
		/// \param T12
		/// \return
		double transferResidual (const Pt2DVec &pts1, const Pt2DVec &pts2, const Transform2D &T12 = Transform2D::Identity());
		
		/// sampson residual of one pair point under transform
		/// \param pt1
		/// \param pt2
		/// \param T12
		/// \return
		double sampsonResidual (const Pt2D &pt1, const Pt2D &pt2, const Transform2D &T12 = Transform2D::Identity());
		
		/// sampson residual of some pairs point  under transform
		/// \param pts1
		/// \param pts2
		/// \param T12
		/// \return
		double sampsonResidual (const Pt2DVec &pts1, const Pt2DVec &pts2, const Transform2D &T12 = Transform2D::Identity());
		
	}
}


#endif //EXAMPLE_ALGEBA_H
