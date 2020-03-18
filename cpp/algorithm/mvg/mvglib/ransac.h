//
// Created by books on 19-10-12.
//

#ifndef EXAMPLE_RANSAC_H
#define EXAMPLE_RANSAC_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <random>
#include <set>
#include <ctime>

#include "mvg_def.h"
#include "algeba.h"
#include "dlt.h"


namespace mvg
{
	
	namespace ransac
	{
		
		Transform2D solveProjective2D (Pt2DVec &pts1, Pt2DVec &pts2, int maxRun = 1000);
		
		Transform2D solveProjective2DAdaptive (Pt2DVec &pts1, Pt2DVec &pts2);
	}
	
}


#endif //EXAMPLE_RANSAC_H
