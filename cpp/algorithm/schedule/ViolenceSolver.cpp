#include "ViolenceSolver.h"
#include <cassert>

template <typename T>
void perm(int sIndex, int eIndex, std::vector<T> vec, std::vector<std::vector<T>> &result)
{
    if (sIndex == eIndex)
    {
        result.push_back(vec);
        return;
    }

    for (int i = sIndex; i < eIndex; i++)
    {
        std::swap(vec[sIndex], vec[i]);
        perm(sIndex + 1, eIndex, vec, result);
        std::swap(vec[i], vec[sIndex]);
    }
};

ViolenceSolver::ViolenceSolver(const CostMap &costMap, int maxNum) : ScheduleSolver(costMap)
{
    init(maxNum);
}

ViolenceSolver::~ViolenceSolver()
{
}

void ViolenceSolver::init(int maxNum)
{
    // m_orderTable[std::make_tuple(1, 0)] = std::vector<Order>(1, Order({1, -1}));

    for (int num = 1; num <= maxNum; num++)
    {
        std::vector<int> sNode;
        for (int j = 1; j <= num; j++)
            sNode.push_back(j);

        /// perm list by snode
        std::vector<Order> sOrderPerm;
        perm(0, num, sNode, sOrderPerm);

        std::vector<Order> orderTable;
        for (const Order &sOrder : sOrderPerm)
        {
            std::vector<Order> curOrderTable;
            Order::const_reverse_iterator crIterOrder;
            for (crIterOrder = sOrder.rbegin(); crIterOrder != sOrder.rend(); crIterOrder++)
            {
                int tmpSNode = *crIterOrder;
                if (curOrderTable.empty())
                {
                    Order tmpOrder;
                    tmpOrder.push_back(tmpSNode);
                    tmpOrder.push_back(-tmpSNode);
                    curOrderTable.push_back(tmpOrder);
                    continue;
                }

                std::vector<Order> tmpOrderTable;
                for (const Order &curOrder : curOrderTable)
                {
                    for (int index = 0; index < curOrder.size(); index++)
                    {
                        Order tmpOrder;
                        tmpOrder.assign(curOrder.begin(), curOrder.end());
                        tmpOrder.insert(tmpOrder.begin() + index, -tmpSNode);
                        tmpOrder.insert(tmpOrder.begin(), tmpSNode);
                        tmpOrderTable.push_back(tmpOrder);
                    }

                    {
                        Order tmpOrder;
                        tmpOrder.assign(curOrder.begin(), curOrder.end());
                        tmpOrder.push_back(-tmpSNode);
                        tmpOrder.insert(tmpOrder.begin(), tmpSNode);
                        tmpOrderTable.push_back(tmpOrder);
                    }
                }

                curOrderTable.swap(tmpOrderTable);
            }
            orderTable.insert(orderTable.end(), curOrderTable.begin(), curOrderTable.end());
        }

        m_orderTable[std::make_tuple(num, 0)] = orderTable;

        /// if still has arrive_num customer in car
        for (int arrive_num = 1; arrive_num + num <= maxNum; arrive_num++)
        {
            assert(m_orderTable.find(std::make_tuple(num, arrive_num - 1)) != m_orderTable.end());

            std::vector<Order> prevOrderTable = m_orderTable[std::make_tuple(num, arrive_num - 1)];
            int arriveVal = -(num + arrive_num);
            std::vector<Order> curOrderTable;
            for (Order prevOrder : prevOrderTable)
            {
                for (int i = 0; i < prevOrder.size(); i++)
                {
                    Order curOrder;
                    curOrder.assign(prevOrder.begin(), prevOrder.end());
                    curOrder.insert(curOrder.begin() + i, arriveVal);
                    curOrderTable.push_back(curOrder);
                }
            }
            m_orderTable[std::make_tuple(num, arrive_num)] = curOrderTable;
        }
    }
}

void ViolenceSolver::printInfo()
{
    size_t fullSize = 0;
    for (const std::pair<std::tuple<int, int>, std::vector<Order>> &pair : m_orderTable)
    {
        std::cout << "**********\t" << std::get<0>(pair.first) << "\t" << std::get<1>(pair.first) << "\t" << pair.second.size() << "\t************" << std::endl;
        for (const Order &order : pair.second)
        {
            for (const int &val : order)
            {
                std::cout << val << "\t";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        fullSize += pair.second.size();
    }
    std::cout << "Full Size " << fullSize << std::endl;
}

NodeList ViolenceSolver::solve(const Tasks &tasks, const Tasks &old_tasks)
{
    assert(!tasks.empty());

    /// [order val, node val]
    std::map<int, int> flectionMap;
    for (size_t i = 0; i < tasks.size(); i++)
    {
        int val = i + 1;
        flectionMap[val] = tasks[i].sNode;
        flectionMap[-val] = tasks[i].eNode;
    }
    for (size_t i = 0; i < old_tasks.size(); i++)
    {
        int val = tasks.size() + i + 1;
        flectionMap[val] = 0;
        flectionMap[-val] = old_tasks[i].eNode;
    }

    double bestCost = std::numeric_limits<double>::max();
    NodeList bestNodeList;
    std::vector<Order> curOrderTable = m_orderTable[std::make_tuple(tasks.size(), old_tasks.size())];

    /// update table because some task only has eNode

    for (const Order &curOrder : curOrderTable)
    {
        NodeList tmpNodeList;
        for (const int &tmp : curOrder)
        {
            tmpNodeList.push_back(flectionMap[tmp]);
        }

        bool findAll = true;
        double tmpCost = 0;
        for (int i = 1; i < tmpNodeList.size(); i++)
        {
            int sNode = tmpNodeList[i - 1];
            int eNode = tmpNodeList[i];
            std::tuple<int, int> nodeTuple = std::make_tuple(sNode, eNode);
            if (findAll && m_costMap.find(nodeTuple) != m_costMap.end())
            {
                tmpCost += m_costMap.at(nodeTuple);
            }
            else
            {
                findAll = false;
            }
        }

        if (findAll && tmpCost < bestCost)
        {
            bestCost = tmpCost;
            bestNodeList = tmpNodeList;
        }
    }

    return bestNodeList;
}
