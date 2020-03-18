#include <iostream>
#include <fstream>

#include <gtest/gtest.h>

void print(std::ostream &os = std::cout)
{
    os << "Hello" << std::endl;
}

TEST(CPP_IOS, iostream_parameter)
{
    print(std::cout);
}

class A
{
public:
    int a;
    int b;
    std::string c;

    friend std::ostream &operator<<(std::ostream &os, const A &a)
    {
        os << a.a << "\t" << a.b << "\t" << a.c;
        return os;
    }
    friend std::istream &operator>>(std::istream &is, A &a)
    {
        is >> a.a >> a.b >> a.c;
        return is;
    }
};

TEST(CPP_IOS, iostream_operator_reload)
{
    A a;
    a.a = 10;
    a.b = 11;
    a.c = "books";
    std::cout << a << std::endl;
}