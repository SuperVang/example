#include <chrono>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

#include <gtest/gtest.h>

TEST(CPP_CONTAINER, vector_key_map)
{
    std::map<std::vector<int>, std::vector<int>> nodeMaps;
    for (int i = 0; i < 10; i++)
    {
        std::vector<int> tmpKey;
        for (int j = i; j < 10; j++) tmpKey.push_back(j);
        nodeMaps[tmpKey] = std::vector<int>(i, 0);
    }

    std::map<std::vector<int>, std::vector<int>>::iterator iter;
    for (iter = nodeMaps.begin(); iter != nodeMaps.end(); iter++)
    {
        std::vector<int> &tmpVal = iter->second;
        EXPECT_EQ(10, iter->first.size() + iter->second.size());
    }
}

TEST(CPP_CONTAINER, pointer_value_map)
{
    struct PV
    {
        int id;
        std::vector<int> vec;
    };
    std::map<int, PV *> pvMap;
    int mapNum = 1000000;
    for (int i = 0; i < mapNum; i++)
    {
        PV *tmp = new PV();
        tmp->id = i;
        tmp->vec = std::vector<int>();
        pvMap[tmp->id] = tmp;
    }

    for (int i = 0; i < 10000; i++)
    {
        pvMap[i % mapNum]->vec.push_back(i);
    }
}

TEST(CPP_CONTAINER, tuple_key_set)
{
    std::set<std::tuple<int, int>> sets;
    for (int i = 0; i < 100; i++) sets.insert(std::make_tuple(i % 10, i % 10));
    EXPECT_EQ(10, sets.size());
    for (const std::tuple<int, int> &t : sets) EXPECT_EQ(std::get<0>(t), std::get<1>(t));
}

TEST(CPP_CONTAINER, vector_assign_from_set)
{
    std::set<std::tuple<int, int>> sets;
    for (int i = 0; i < 100; i++) sets.insert(std::make_tuple(i % 10, i % 10));

    std::vector<std::tuple<int, int>> vec;
    vec.assign(sets.begin(), sets.end());
    EXPECT_EQ(10, vec.size());
    for (const std::tuple<int, int> &t : vec) EXPECT_EQ(std::get<0>(t), std::get<1>(t));
}

TEST(CPP_CONTAINER, set_insert_from_vector)
{
    std::vector<int> vec;
    for (int i = 0; i < 100; i++) vec.push_back(i % 10);

    std::set<int> sets;
    sets.insert(vec.begin(), vec.end());
    EXPECT_EQ(10, sets.size());
}

TEST(CPP_STRING, endl_char)
{
    std::string str;
    for (int i = 0; i < 10; i++)
    {
        if (i % 2 == 0)
            str.push_back('s');
        else
            str.push_back('\0');
    }
    EXPECT_EQ(10, str.length());

    const char *data = str.data();
    EXPECT_STREQ("s\0s\0s\0s\0s\0", data);

    const char *cStr = str.c_str();
    EXPECT_STREQ("s\0s\0s\0s\0s\0", cStr);

    std::string str2 = data;
    EXPECT_EQ(1, str2.length());
    EXPECT_STREQ("s", str2.c_str());
}

TEST(CPP_STRING, find_first_of)
{
    std::vector<std::string> vec = {"0008--B008--1--1566974281665", "0024--B024--1--1566974281669", "0028--B028--1--1566974281670"};

    for (std::string str : vec)
    {
        std::string det = "--";
        std::string::size_type first = str.find_first_of(det.c_str());
        std::string::size_type second = str.find_first_of(det.c_str(), first + det.length());
        std::string::size_type third = str.find_first_of(det.c_str(), second + det.length());

        std::string firstStr = str.substr(0, first);
        std::string secondStr = str.substr(first + det.length(), second - first - det.length());
        std::string thirdStr = str.substr(second + det.length(), third - second - det.length());
        std::string fourthStr = str.substr(third + det.length());

        int id = std::atoi(firstStr.c_str());

        std::cout << str << "\t" << first << "\t" << second << "\t" << third << std::endl;
        std::cout << firstStr << "\t" << secondStr << "\t" << thirdStr << "\t" << fourthStr << std::endl;
        std::cout << id << std::endl;
    }
}

TEST(CPP_CONTAINER, list_erase)
{
    class T
    {
    public:
        T() {}

        ~T() { printf("T Has Been Destruct!\n"); }
    };

    std::list<T *> list;
    list.push_back(new T());
    T *tmp = list.front();
    list.erase(list.begin());
    EXPECT_TRUE(list.empty());
    std::cout << "List Is Empty" << std::endl;
    delete tmp;
}

TEST(CPP_CONTAINER, set_to_vector)
{
    std::set<int> s;
    for (int i = 0; i < 5; i++) s.insert(i);
    std::vector<int> vec;
    vec.assign(s.begin(), s.end());
    EXPECT_EQ(s.size(), vec.size());
    for (const int val : vec) EXPECT_TRUE(s.find(val) != s.end());
}

TEST(CPP_CONTAINER, vector_erase)
{
    std::vector<int> vec;
    for (int i = 0; i < 10; i++)
    {
        vec.push_back(i);
    }

    vec.erase(vec.end() - 1);

    EXPECT_EQ(9, vec.size());
    EXPECT_EQ(8, vec.back());
}

TEST(CPP_CONTAINER, count_on_tuple)
{
    std::set<std::tuple<int, int>> set;
    for (int i = 0; i < 1000; i++)
    {
        //        map[std::tuple<int, int>(i, i)] = i * 32.4;
        set.insert(std::make_tuple(i, i));
    }

    std::chrono::steady_clock::time_point tp1 = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000; i++)
    {
        int index = rand() % 1000;
        set.count(std::tuple<int, int>(index, index));
    }
    std::chrono::steady_clock::time_point tp2 = std::chrono::steady_clock::now();
    std::chrono::milliseconds span_init = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1);

    std::cout << "Count Span : " << span_init.count() << std::endl;
}

TEST(CPP_CONTAINER, stack_pop_push)
{
    std::stack<int> s;

    for (int i = 0; i < 10; i++)
    {
        s.push(i);
    }
    while (!s.empty())
    {
        std::cout << s.top() << std::endl;
        s.pop();
    }
}

/// 使用栈实现数据库sql语句中的嵌套where
TEST(CPP_CONTAINER, stack_recursive)
{
    enum Type
    {
        AND = 1,
        OR = 2,
        EQ = 3,
        GT,
        LT
    };

    typedef std::tuple<std::string, std::string, Type> Cond;

    std::stack<Cond> where;
    where.push(Cond("id", "10", EQ));
    where.push(Cond("id", "20", LT));
    where.push(Cond("", "", AND));

    std::string sql;
    while (!where.empty())
    {
        Cond tmp = where.top();
        where.pop();
    }
}

TEST(CPP_CONTAINER, map_erase)
{
    std::map<std::tuple<char, char>, int> mymap;
    std::map<std::tuple<char, char>, int>::iterator it;

    // insert some values:
    mymap[{'a', 'a'}] = 10;
    mymap[{'b', 'b'}] = 20;
    mymap.erase({'a', 'z'});
    mymap.erase({'a', 'a'});

    //    it=mymap.find('z');
    //    mymap.erase (it);                   // erasing by iterator

    // show content:
    for (it = mymap.begin(); it != mymap.end(); ++it)
        std::cout << std::get<0>(it->first) << " => " << it->second << '\n';
}
