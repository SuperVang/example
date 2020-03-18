#ifndef _BOOKS_PYBIND11_CPP_H
#define _BOOKS_PYBIND11_CPP_H

#include <iostream>

class Cpp
{
public:

    Cpp()
    { m_num = 1; };

    int getNum()
    {
        return m_num;
    }

private:
    int m_num;

};

#endif