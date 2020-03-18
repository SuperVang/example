
include_directories("/usr/include/eigen3")

file(GLOB_RECURSE ml_src
        "${CMAKE_CURRENT_SOURCE_DIR}/ml_lib/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/ml_lib/*.h")

add_library(ml_lib ${ml_src})

add_executable(ml_test
        linear_regression_test.cpp
        logistic_regression_test.cpp)
target_link_libraries(ml_test ml_lib)