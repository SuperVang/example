
include_directories("/usr/include/eigen3/")

add_executable(eigen_test
        matrix_test.cpp
        geometry_test.cpp
        spline_test.cpp)