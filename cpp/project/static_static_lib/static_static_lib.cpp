//
// Created by books on 19-12-11.
//

#include "static_static_lib.h"

#include <iostream>

void static_static_lib::print() const
{
    static_lib a;
    std::cout<<"This is static_static_lib!"<<std::endl;
    a.print();
}

