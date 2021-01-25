//
// Created by books on 2020/8/18.
//

#include <iostream>

#include <gtest/gtest.h>

TEST(CPP_OPEATOR, overload_ne)
{
  class A
  {
   public:
    int a;

    bool operator==(const A &other) const { return a == other.a; }
    bool operator!=(const A &other) const { return a != other.a; }
  };

  A a1, a2;
  a1.a = 1;
  a2.a = 1;

  std::cout << (a1 != a2) << std::endl;
}
