#include <iostream>
#include <chrono>
#include <vector>
#include <boost/pool/simple_segregated_storage.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <gtest/gtest.h>

TEST(MEMORYPOOL,boost_simple_pool)
{
    boost::simple_segregated_storage<std::size_t> storage;
    std::vector<char> v(1024);
    storage.add_block(&v.front(), v.size(), 256);

    int *i = static_cast<int *>(storage.malloc());
    *i = 1;

    int *j = static_cast<int *>(storage.malloc_n(1, 512));
    j[10] = 2;

    storage.free(i);
    storage.free_n(j, 1, 512);
}

TEST(MEMORYPOOL,boost_pool)
{
    const int cbElement = sizeof(int); // 每次分配的块的大小。
    boost::pool<> alloc(cbElement);

    std::chrono::steady_clock::time_point tp1 = std::chrono::steady_clock::now();
    for (int i = 0; i < 10000; ++i)
    {
        // int* p = (int*)alloc.malloc();
        int *p = (int *)alloc.ordered_malloc(100);
        // int * p=(int *)alloc.malloc_n(1,100);
        p[99] = 1;

        // 不要奇怪这里没有指定要申请的内存大小，事实上内存池仅分配大小相同的内存块。
        // 所以，分配的字节数在构造pool组件的时候已经指定好了。

        // alloc.free(p);

        // 释放内存块，交还给pool，不是返回给系统。
        // 如果愿意，你也可以不free该内存，pool组件析构时将清空管理的所有内存。
    }
    std::chrono::steady_clock::time_point tp2 = std::chrono::steady_clock::now();
    std::chrono::milliseconds span = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1);
    std::cout << "Time Span : " << span.count() << std::endl;
}

TEST(MEMORYPOOL,boost_object_pool)
{
    class Obj
    {
    public:
        int x;
        std::string name;
        char *data;
    };

    boost::object_pool<Obj> alloc;

    std::chrono::steady_clock::time_point tp1 = std::chrono::steady_clock::now();
    for (int i = 0; i < 5000000; ++i)
    {
        void *mem = alloc.malloc();
        Obj *obj1 = new (mem) Obj();
        obj1->data = (char*)"dasa";
        // 注意；X的构造函数不会被调用，仅仅是分配大小为sizeof(Obj)的内存块。
        // 如果需要调用构造函数（像new一样），应该调用construct。
        // Obj* obj1 = new(mem) Obj(arg1, arg2, ...);
        // 调用了placement new，从而得到真正意义上的Obj类型的对象。

        Obj *obj2 = alloc.construct();
        obj2->data = (char*)"ssss";

        alloc.destroy(obj2);

        // 你可以忘记释放obj1，boost::object_pool会记得这件事情。
    }
    std::chrono::steady_clock::time_point tp2 = std::chrono::steady_clock::now();
    std::chrono::milliseconds span = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1);
    std::cout << "Time Span : " << span.count() << std::endl;
}

TEST(MEMORYPOOL,boost_pool_alloc)
{
    std::chrono::steady_clock::time_point tp1 = std::chrono::steady_clock::now();
    std::vector<int, boost::pool_allocator<int>> v;
    for (int i = 0; i < 9000000; ++i)
        v.push_back(13);
    std::chrono::steady_clock::time_point tp2 = std::chrono::steady_clock::now();
    std::chrono::milliseconds span = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1);
    std::cout << "Time Span : " << span.count() << std::endl;
}

TEST(MEMORYPOOL,boost_singleton_pool)
{
    /*    The Tag template parameter uniquelyidentifies this pool and allows
      different unbounded sets of singleton pools to exist.
      For example, the pool allocators use two tag classes to ensure that the
      two different allocator types never share the same underlying singleton pool.
      Tag is never actually used by singleton_pool.*/
    struct MyPoolTag
    {
    };

    typedef boost::singleton_pool<MyPoolTag, sizeof(int)> my_pool;

    std::chrono::steady_clock::time_point tp1 = std::chrono::steady_clock::now();
    for (int i = 0; i < 50000000; ++i)
    {
        int *p = (int *)my_pool::malloc(); // 这里就和boost::pool不太一样了。
    }
    my_pool::purge_memory(); // 释放my_pool申请的所有内存
    std::chrono::steady_clock::time_point tp2 = std::chrono::steady_clock::now();
    std::chrono::milliseconds span = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1);
    std::cout << "Time Span : " << span.count() << std::endl;
}