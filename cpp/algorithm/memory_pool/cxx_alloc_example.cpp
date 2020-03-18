#include <iostream>
#include <set>
#include <map>
#include <list>
#include <deque>
#include "Alloc/AllocType.h"
#include "Alloc/AllocContainer.h"

#include <gtest/gtest.h>

TEST(MEMORYPOOL,cxx_auto_alloc)
{
    AutoAlloc alloc;

    int *intObj = MEMORY_NEW(alloc, int);
    int *intObjWithArg = MEMORY_NEW(alloc, int)(10);
    int *intArray = MEMORY_NEW_ARRAY(alloc, int, 100);
    int *intBuf = MEMORY_ALLOC(alloc, int);
    int *intArrayBuf = MEMORY_ALLOC_ARRAY(alloc, int, 100);

    AutoAlloc *subAlloc = MEMORY_NEW(alloc, AutoAlloc);

    int *e = MEMORY_NEW(*subAlloc, int);
}

TEST(MEMORYPOOL,cxx_scoped_alloc)
{
    BlockPool recycle;
    ScopedAlloc alloc(recycle);

    int *intObj = MEMORY_NEW(alloc, int);
    int *intObjWithArg = MEMORY_NEW(alloc, int)(10);
    int *intArray = MEMORY_NEW_ARRAY(alloc, int, 100);
    int *intBuf = MEMORY_ALLOC(alloc, int);
    int *intArrayBuf = MEMORY_ALLOC_ARRAY(alloc, int, 100);

    ScopedAlloc *suballoc = MEMORY_NEW(alloc, ScopedAlloc)(alloc);

    int *e = MEMORY_NEW(*suballoc, int);
}

enum
{
    Count = 100
};

TEST(MEMORYPOOL,cxx_alloc_deque)
{
    printf("\n===== Deque (ScopedAlloc) =====\n");
    BlockPool recycle;
    ScopedAlloc alloc(recycle);
    std::deque<int, StlAlloc<int>> s(alloc);
    for (int i = 0; i < Count; ++i)
        s.push_back(i);
}

TEST(MEMORYPOOL,cxx_alloc_list)
{
    printf("\n===== List (ScopedAlloc) =====\n");
    BlockPool recycle;
    ScopedAlloc alloc(recycle);
    std::list<int, StlAlloc<int>> s(alloc);
    for (int i = 0; i < Count; ++i)
        s.push_back(i);
}

TEST(MEMORYPOOL,cxx_alloc_set)
{
    printf("\n===== Set (ScopedAlloc) =====\n");
    BlockPool recycle;
    ScopedAlloc alloc(recycle);
    std::set<int, std::less<int>, StlAlloc<int>> s(std::less<int>(), alloc);
    for (int i = 0; i < Count; ++i)
        s.insert(i);
}

TEST(MEMORYPOOL,cxx_alloc_map)
{
    printf("\n===== Map (ScopedAlloc) =====\n");
    // BlockPool recycle;
    ScopedAlloc alloc;
    std::map<int, int, std::less<int>, StlAlloc<int>> s(std::less<int>(), alloc);
    for (int i = 0; i < Count; ++i)
        s.insert(std::pair<int, int>(i, i));
}

TEST(MEMORYPOOL,cxx_alloc_efficiency)
{
    AutoAlloc alloc;
    const int num = 100000000;
    std::cout << "Start Allocate Memory" << std::endl;
    for (int i = 0; i < num; i++)
    {
        int *tt = (int *)alloc.allocate(sizeof(int));
    }
    std::cout << "Allocate Memory Done" << std::endl;

    std::cout << "Start Free Memory" << std::endl;
    alloc.clear();
    std::cout << "Free Memory Done" << std::endl;

    std::cout << "Start Allocate Memory" << std::endl;
    for (int i = 0; i < 2 * num; i++)
    {
        int *tt = (int *)alloc.allocate(sizeof(int));
    }
    std::cout << "Allocate Memory Done" << std::endl;

    std::cout << "Start Free Memory" << std::endl;
    alloc.clear();
    std::cout << "Free Memory Done" << std::endl;
}
