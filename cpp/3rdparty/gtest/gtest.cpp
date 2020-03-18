#include <iostream>
#include <algorithm>
#include <memory>
#include <numeric>
#include <gtest/gtest.h>

TEST(MyTest, SUM)
{
    std::vector<int> vec{1, 2, 3, 4, 5};
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    EXPECT_EQ(15, sum);
}

TEST(MyTest, PRODUCT)
{
    std::vector<int> vec{1, 2, 3};
    int product = 1;
    for (const int val : vec)
        product *= val;
    EXPECT_EQ(5, product);
}
