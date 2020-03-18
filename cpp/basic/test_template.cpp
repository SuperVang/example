#include <gtest/gtest.h>
#include <iostream>
#include <vector>

struct TTT {
    struct A {};

    struct B {};

    std::string str;

    template <typename T>
    void push(T t) {
        if (std::is_same<typename std::decay<T>::type, A>::value) {
            str += "A";
        } else if (std::is_same<typename std::decay<T>::type, B>::value) {
            str += "B";
        }
    }
};

TEST(CPP_TEMPLATE, is_same_function) {
    TTT t;
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            TTT::A a;
            t.push(a);
        } else {
            TTT::B b;
            t.push(b);
        }
    }
    EXPECT_STREQ("ABABABABAB", t.str.c_str());
}

template <typename T>
struct A1 {
    T min;
    T max;
    A1() = default;

    bool contain(const T &val) {
        return val >= min && val <= max;
    }
};

TEST(CPP_TEMPLATE,two_template)
{
    A1<double> a1;
    a1.min=20.0;
    a1.max=40.0;

    EXPECT_TRUE(a1.contain(30));
    EXPECT_FALSE(a1.contain(10));
}
