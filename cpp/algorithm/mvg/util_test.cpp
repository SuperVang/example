//
// Created by books on 19-10-11.
//

#include <iostream>
#include "gtest/gtest.h"

#include "mvglib/mvg_util.h"



TEST(MVG_UTIL, build_random_projective)
{
	mvg::Transform2D t = mvg::util::buildRandomProjective2D();
}
