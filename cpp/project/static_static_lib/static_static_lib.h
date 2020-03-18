//
// Created by books on 19-12-11.
//

#ifndef EXAMPLE_STATIC_B_H
#define EXAMPLE_STATIC_B_H
#include <iostream>
#include "../static_lib/static_lib.h"

class static_static_lib
{
public:
    void print()const;
};
static void do_more_dirty();
static void do_dirty_work(){
//    do_more_dirty();
    std::cout<<"Dirty Work"<<std::endl;
}



#endif //EXAMPLE_STATIC_B_H
