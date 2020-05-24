
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>

#include "cpp.h"

// int add(int a,int b)
// {
//     return a+b;
// }

// PYBIND11_MODULE(test_pybind,m)
// {
// //    pybind11::class_<Cpp>(m,"Cpp")
// //        .def(pybind11::init<>())
// //        .def("getNum",&Cpp::getNum);

//     m.doc()="books first try";
//     m.def("add",&add,"A function which adds two numbers");
// }

struct A
{
    int id;
    int age;
};

static A *a = new A();

bool getAdultA(int id, int age, A &a)
{
    if (age > 10)
    {
        a.id = id;
        a.age = age;
        return true;
    }
    return false;
}

PYBIND11_MODULE(test_pybind, m)
{
    pybind11::class_<A>(m, "A").def(pybind11::init<>()).def_readonly("id", &A::id).def_readonly("age", &A::age);

    m.def("getAdultA", [](int id, int age) {
        bool flag = getAdultA(id, age, *a);
        if (flag)
            return a;
        else
            return (A *)nullptr;
    });
}