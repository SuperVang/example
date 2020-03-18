//
// Created by books on 19-12-12.
//

#include <iostream>
#include <string>
#include <vector>
#include <gtest/gtest.h>

enum CondType
{
    CondType_EQ = 0,
    CondType_NE,
    CondType_GT,
    CondType_GE,
    CondType_LT,
    CondType_LE,
    CondType_LIKE
};

typedef std::tuple<std::string, std::string, CondType> DBCond;

class WhereSql
{

public:
    WhereSql() = default;

    WhereSql(DBCond cond)
    {                std::string key = std::get<0>(cond);
        std::string val = std::get<1>(cond);
        std::string op = "";
        switch (std::get<2>(cond))
        {
            case CondType_EQ:
                op = " = ";
                break;
            case CondType_GE:
                op = " >= ";
                break;
            case CondType_GT:
                op = " > ";
                break;
            case CondType_LE:
                op = " <= ";
                break;
            case CondType_LT:
                op = " < ";
                break;
            case CondType_LIKE:
                op = " like ";
                break;
            case CondType_NE:
                op = " != ";
                break;
            default:
                op = " = ";
        }
        m_data = key + op + val;}

    WhereSql(std::string data) : m_data(data)
    {}

    ~WhereSql() = default;

    std::string operator()() const
    {
        return "(" + m_data + ")";
    }

    WhereSql operator+(const WhereSql &other)
    {
        return WhereSql(WhereSql(m_data + " and " + other.m_data)());
    }

    WhereSql operator*(const WhereSql &other)
    {
        return WhereSql(WhereSql(m_data + " or " + other.m_data)());
    }


    std::string m_data;
};


TEST(DB_WHERE, recursive_where)
{
    DBCond cond1("id", "1", CondType_EQ);
    DBCond cond2("id", "2", CondType_NE);
    DBCond cond3("id", "3", CondType_LE);
    DBCond cond4("id", "4", CondType_LT);
    DBCond cond5("id", "5", CondType_GT);
    DBCond cond6("id", "6", CondType_LIKE);
    WhereSql sql = WhereSql(cond1) * WhereSql(cond2) + (WhereSql(cond3) + WhereSql(cond4) * WhereSql(cond5))*WhereSql(cond6);
    std::cout << sql.m_data << std::endl;
}


