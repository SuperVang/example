//
// Created by books on 19-12-11.
//

#include "static_shared_lib.h"
#include "../static_lib/static_lib.h"
#include <iostream>

void static_shared_lib::print() const
{
    std::cout<<"This is static shared lib"<<std::endl;
    static_lib a;
    a.print();
}
