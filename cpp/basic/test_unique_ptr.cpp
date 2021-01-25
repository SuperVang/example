
#include <iostream>
#include <memory>

#include <gtest/gtest.h>

#include "A.h"

TEST(CPP_UNIQUE_PTR, forward_declare)
{
  UNIQUE::A a;
  // UNIQUE::A a;
}

TEST(CPP_UNIQUE_PTR, init_with_pointer)
{
  {
    int *a = new int(10);
    std::unique_ptr<int> ptr;

    ptr = std::make_unique<int>(*a);
    delete a;
    EXPECT_EQ(*ptr, 10);
  }

  struct PPP {
    int a = 0;
    double b = 0;
    std::string p = "";
  };

  {
    PPP *p = new PPP();
    p->a = 10;
    p->b = 12.98;
    p->p = "cai";

    std::unique_ptr<PPP> ptr = std::make_unique<PPP>(*p);

    delete p;

    EXPECT_EQ(ptr->a, 10);
    EXPECT_EQ(ptr->b, 12.98);
    EXPECT_EQ(ptr->p, "cai");
  }
}

TEST(CPP_UNIQUE_PTR, init_with_base_pointer)
{
  class Base
  {
   public:
    Base() { std::cout << "init base" << std::endl; }
    Base(const Base &b) { std::cout << "copy base" << std::endl; }

    virtual void printf() { std::cout << "print base" << std::endl; }
  };
  class A : public Base
  {
   public:
    A() { std::cout << "init a" << std::endl; }

    A(const A &b) { std::cout << "copy a" << std::endl; }

    virtual void printf() override { std::cout << "print a" << std::endl; }
  };

  A *a = new A();
  std::cout << "*************" << std::endl;
  std::unique_ptr<Base> ptr1 = std::make_unique<A>(*a);
  std::cout << "*************" << std::endl;
  std::unique_ptr<Base> ptr2 = std::make_unique<Base>(*a);
  std::cout << "*************" << std::endl;
  delete a;

  ptr1->printf();
  std::cout << "*************" << std::endl;
  ptr2->printf();
}
