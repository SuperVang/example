//
// Created by books on 2020/5/8.
//

#include <iostream>
#include <memory>

#include <gtest/gtest.h>

namespace AA {
class A;

class B
{
public:
    B() = default;
    ~B() = default;
    std::shared_ptr<A> a;
};
} // namespace AA

TEST(CPP_SHARED_PTR, forward_declare) { AA::B b; }

namespace AA {
class A
{
public:
    A() { std::cout << "Initilize A" << std::endl; }
    A(const A &) { std::cout << "Copy A" << std::endl; }
    A(const A &&) { std::cout << "Move A" << std::endl; }
    ~A() { std::cout << "Release A" << std::endl; }
    void print() { std::cout << "A" << std::endl; }
};
} // namespace AA

void add(std::vector<std::shared_ptr<AA::A>> &vec, std::shared_ptr<AA::A> ptr) { vec.push_back(ptr); }

TEST(CPP_SHARED_PTR, init_with_origin_ptr)
{
    AA::A *a = new AA::A();
    std::shared_ptr<AA::A> ptr(a);

    std::vector<std::shared_ptr<AA::A>> vec;
    vec.push_back(ptr);
    vec.push_back(ptr);
    add(vec, ptr);
    std::cout << ptr.use_count() << std::endl;
    vec.clear();
    std::cout << ptr.use_count() << std::endl;
}

TEST(CPP_SHARED_PTR, init_with_make)
{
    AA::A a;

    std::shared_ptr<AA::A> ptr = std::make_shared<AA::A>(a);

    std::vector<std::shared_ptr<AA::A>> vec;
    vec.push_back(std::make_shared<AA::A>(a));
    vec.push_back(std::make_shared<AA::A>(a));

    std::shared_ptr<AA::A> ptr2 = ptr;
}

TEST(CPP_SHARED_PTR, get_nullptr)
{
    std::shared_ptr<int> ptr = nullptr;
    int *raw = ptr.get();
    ASSERT_FALSE(raw);
}
