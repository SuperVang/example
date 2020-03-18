#include <iostream>
#include <gtest/gtest.h>

#include "ViolenceSolver.h"
#include "solver_util.h"

TEST(ALGORITHM_SCHEDULE, violence_schedule_solver)
{
    CostMap costMap = initRandCostMap(100);
    ViolenceSolver solver(costMap, 4);
    solver.printInfo();
}
