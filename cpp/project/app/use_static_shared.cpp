//
// Created by books on 19-12-11.
//

#include <iostream>

#include "../static_lib/static_lib.h"
#include "../static_shared_lib/static_shared_lib.h"

int main(){
    static_lib a;
    a.print();

    static_shared_lib aa;
    aa.print();
}