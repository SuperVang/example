
find_package(pybind11)
if(pybind11_FOUND)
    include_directories(${pybind11_DIRS})
    message(STATUS "PyBind Inc : ${pybind11_INCLUDE_DIRS}")
    pybind11_add_module(test_pybind wrapper.cpp cpp.h cpp.cpp)
endif()