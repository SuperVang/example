#include <iostream>
#include <vector>
#include <gtest/gtest.h>

TEST(STL_CLASS, virtual_function)
{
    class Base
    {
    public:
        int base;
        virtual std::string type()
        {
            return "Base";
        }
        virtual std::string onlyBase()
        {
            return type();
        }
    };

    class A : public Base
    {
    public:
        int a;
        std::string type() override
        {
            return "A";
        }
    };

    class B : public A
    {
    public:
        std::string type() override
        {
            return A::type() + "B";
        }
    };

    std::vector<Base *> baseVec;

    for (int i = 0; i < 10; i++)
    {
        A *a = new A();
        a->base = i;
        a->a = i * 2;
        EXPECT_STREQ("A", a->type().c_str());
        EXPECT_STREQ("A", a->onlyBase().c_str());
        baseVec.push_back(a);
    }

    for (int i = 0; i < 10; i++)
    {
        EXPECT_STREQ("A", baseVec[i]->type().c_str());
    }
    for (int i = 0; i < 10; i++)
    {
        A *a = (A *)(baseVec[i]);
        EXPECT_STREQ("A", a->type().c_str());
        EXPECT_EQ(i * 2, a->a);
    }

    Base *b = new B();
    EXPECT_STREQ("AB", b->type().c_str());
}

TEST(CPP_CLASS, recursive_class)
{
    struct Tile
    {
        int id;
        struct link
        {
            int id;
        };
        std::vector<link> links;
    };

    Tile tile;
    tile.id = 3;
    for (int i = 0; i < 10; i++)
    {
        Tile::link link;
        link.id = i;
        tile.links.push_back(link);
    }

    EXPECT_EQ(10, tile.links.size());

    for (int i = 0; i < tile.links.size(); i++)
        EXPECT_EQ(i, tile.links[i].id);
}

TEST(CPP_CLASS, inherit_final)
{
    class Base
    {
    public:
        virtual std::string print()
        {
            return "Base";
        }
    };
    class A : public Base
    {
    public:
        virtual std::string print() final
        {
            return "A";
        }
    };
    class B : public A
    {
    public:
        std::string print(int a)
        {
            return "B";
        }
    };

    Base base;
    A a;
    B b;
    EXPECT_STREQ("Base", base.print().c_str());
    EXPECT_STREQ("A", a.print().c_str());
    EXPECT_STREQ("B", b.print(1).c_str());
}

TEST(CPP_CLASS, private_virtual)
{
    class Base
    {
    public:
        virtual int print()
        {
            return p();
        }

    private:
        virtual int p()
        {
            return 1;
        }
    };
    class A : public Base
    {
    public:
        virtual int print()
        {
            return Base::print();
        }
    };

    Base *b = new A();
    EXPECT_EQ(1, b->print());
}