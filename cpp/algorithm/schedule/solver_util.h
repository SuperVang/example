#ifndef SOLVER_UTIL_H
#define SOLVER_UTIL_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <ctime>
#include <set>
#include <list>

#include "ViolenceSolver.h"

CostMap initRandCostMap(int num)
{
    CostMap costMap;
    std::default_random_engine e(std::time(nullptr));
    std::uniform_real_distribution<> uniform(10, 1000);

    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
        {
            costMap[std::make_tuple(i, j)] = i == j ? 0 : uniform(e);
        }
    }
    return costMap;
}

Tasks initRandTasks(int num, const CostMap &costMap)
{
    std::set<int> nodeSet;
    std::map<int, std::vector<int>> nodeMap;
    for (const std::pair<std::tuple<int, int>, double> &pair : costMap)
    {
        nodeSet.insert(std::get<0>(pair.first));
        nodeSet.insert(std::get<1>(pair.first));
        nodeMap[std::get<0>(pair.first)].push_back(std::get<1>(pair.first));
    }
    std::vector<int> nodes;
    nodes.assign(nodeSet.begin(), nodeSet.end());

    std::default_random_engine e(std::time(nullptr));
    std::uniform_int_distribution<> uniform(0, nodes.size());

    std::vector<Task> tasks;
    for (int i = 0; i < num; i++)
    {
        int sVal = nodes[uniform(e)];
        if (nodeMap.find(sVal) != nodeMap.end())
        {
            int eVal = nodeMap[sVal][uniform(e)];
            tasks.push_back(Task(sVal, eVal));
        }
    }
    return tasks;
}

#endif