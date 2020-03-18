#include <iostream>
#include <gtest/gtest.h>

TEST(MEMORYPOOL, c_malloc_free)
{
    struct link
    {
        link *next;
        char data[1000];
    };

    struct A
    {
        int a;
        int b;
        void print()
        {
            std::cout << "print a" << std::endl;
        }
    };

    const int num = 3000000;
    int block_num = 0;
    link *header = (link *)malloc(sizeof(link));
    header->next = nullptr;

    std::cout << "Start Allocate Memory" << std::endl;
    for (int i = 0; i < num; i++)
    {
        link *tmp = (link *)malloc(sizeof(link));
        tmp->next = header;
        header = tmp;
        block_num++;
    }
    std::cout << "Allocate Memory Done" << std::endl;
    std::cout << "Memory Block Num : " << block_num << std::endl;

    std::cout << "Start Free Memory" << std::endl;
    while (header->next)
    {
        link *tmp = header;
        header = header->next;
        free(tmp);
        block_num--;
    }
    std::cout << "Memory Block Num : " << block_num << std::endl;
    std::cout << "Free Memory Done" << std::endl;

    std::cout << "Start Allocate Memory" << std::endl;
    for (int i = 0; i < 2 * num; i++)
    {
        link *tmp = (link *)malloc(sizeof(link));
        tmp->next = header;
        header = tmp;
        block_num++;
    }
    std::cout << "Allocate Memory Done" << std::endl;
    std::cout << "Memory Block Num : " << block_num << std::endl;

    std::cout << "Start Free Memory" << std::endl;
    while (header->next)
    {
        link *tmp = header;
        header = header->next;
        free(tmp);
        block_num--;
    }
    std::cout << "Memory Block Num : " << block_num << std::endl;
    std::cout << "Free Memory Done" << std::endl;
}