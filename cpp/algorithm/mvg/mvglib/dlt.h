#ifndef _MVG_DLT_H
#define _MVG_DLT_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>

#include "mvg_def.h"
#include "algeba.h"

namespace mvg
{
	namespace dlt
	{

		/// pts2=H*pts1 -> H
		Transform2D solveProjective2D (const Pt2DVec &pts1, const Pt2DVec &pts2);
		
		/// pts2=H*pts1 -> H
		Transform2D solveProjective2DNormalize(const Pt2DVec &pts1,const Pt2DVec &pts2);
		
		Transform2D normalize (const Pt2DVec &pts);
		
		
		
		
	};
}  // namespace mvg
#endif