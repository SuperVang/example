#ifndef VIOLENCE_SOLVER_H
#define VIOLENCE_SOLVER_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <ctime>
#include <set>
#include <list>

#include "ScheduleSolver.h"

class ViolenceSolver : public ScheduleSolver
{
public:
    ViolenceSolver(const CostMap &costMap, int maxNum = 4);
    ~ViolenceSolver();

    void init(int maxNum);

    void printInfo();

    NodeList solve(const Tasks &tasks, const Tasks &old_tasks) override;

protected:
    /// 正数为上车点,负数为下车点
    typedef std::vector<int> Order;
    /// [task num,task order]
    std::map<std::tuple<int, int>, std::vector<Order>> m_orderTable;
};

#endif