#ifndef SCHEDULE_SOLVER_H
#define SCHEDULE_SOLVER_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <ctime>
#include <set>
#include <list>

struct Task
{
    int id;
    int sNode;
    int eNode;
    Task() = default;
    Task(int s, int e)
    {
        static int _id = 1;
        id = _id++;
        sNode = s;
        eNode = e;
    }
};

typedef std::vector<Task> Tasks;
typedef std::map<std::tuple<int, int>, double> CostMap;
typedef std::vector<int> NodeList;

class ScheduleSolver
{
public:
    ScheduleSolver(const CostMap &costMap) : m_costMap(costMap)
    {
    }
    virtual ~ScheduleSolver()
    {
    }

    virtual NodeList solve(const Tasks &tasks, const Tasks &old_tasks)
    {
        return NodeList();
    }

protected:
    CostMap m_costMap;
};

#endif