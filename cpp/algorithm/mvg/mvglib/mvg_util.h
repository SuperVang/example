//
// Created by books on 19-10-11.
//

#ifndef EXAMPLE_MVG_UTIL_H
#define EXAMPLE_MVG_UTIL_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <random>
#include <ctime>

#include "mvg_def.h"

namespace mvg
{
	namespace util
	{
		Pt2DVec buildRandomPts2D (int n);
		
		Pt2DVec buildRandomPts2D (const Pt2DVec &pts, const Transform2D &T);
		
		Pt2DVec buildRandomPts2D (const Pt2DVec &pts, double noise);
		
		Transform2D buildRandomProjective2D ();
	}
	
}  // namespace mvg
#endif  // EXAMPLE_MVG_UTIL_H
