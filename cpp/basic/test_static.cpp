#include <iostream>
#include <gtest/gtest.h>

struct A
{
    static const int a = 8;
    static const std::string str;
};
const std::string A::str = "danshi";

TEST(CPP_STATIC, static_in_class)
{
    EXPECT_STREQ("danshi", (A::str).c_str());
    int tmp = A::a;
    EXPECT_EQ(8, tmp);
}