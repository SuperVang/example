#include <iostream>
#include <map>
#include <algorithm>
#include <gtest/gtest.h>

TEST(CPP_POINTER, container_value)
{
    std::map<int, int *> map;
    for (int i = 0; i < 10; i++)
    {
        int *tmp = new int(i);
        map.insert(std::make_pair(i, tmp));
    }

    std::map<int, int *>::iterator iter;
    for (iter = map.begin(); iter != map.end(); iter++)
    {
        EXPECT_EQ(iter->first, *iter->second);
    }
}

void fill(int **p)
{
    int *tmp = new int[100];
    for (int i = 0; i < 100; i++)
        tmp[i] = i;
    *p = tmp;
}

TEST(CPP_POINTER, pointer_pointer)
{
    int *p;
    fill(&p);

    for (int i = 0; i < 100; i++)
        EXPECT_EQ(i, p[i]);
}

TEST(CPP_POINTER, sort_pointer_vector)
{
    std::vector<int *> vec, vec_sort;
    for (int i = 0; i < 10; i++)
    {
        int *tmp = new int(i);
        vec.push_back(tmp);
        vec_sort.push_back(tmp);
    }

    std::sort(vec_sort.begin(), vec_sort.end(), [](const int *a, const int *b) {
        return *a < *b;
    });

    for (int i = 1; i < 10; i++)
        EXPECT_LE(*(vec_sort[i - 1]), *(vec_sort[i]));

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (vec[i] == vec_sort[j])
            {
                EXPECT_TRUE(vec[i] == vec_sort[j]);
            }
        }
    }
}

TEST(CPP_POINTER, nullptr_size)
{
    int *p;
    EXPECT_EQ(8, sizeof(p));
    int *q = nullptr;
    EXPECT_EQ(8, sizeof(q));
}