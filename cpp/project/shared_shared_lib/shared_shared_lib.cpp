//
// Created by books on 19-12-11.
//

#include "../shared_lib/shared_lib.h"
#include "shared_shared_lib.h"

#include <iostream>

void shared_shared_lib::print() const
{
    std::cout<<"This is shared shared lib2"<<std::endl;
    shared_lib a;
    a.print();

}
