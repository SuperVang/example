
find_package(Ceres)

if(CERES_FOUND)
    include_directories("/usr/include/eigen3/")

    message(STATUS "Ceres Version : ${CERES_VERSION}")
    include_directories(${CERES_INCLUDE_DIRS})
    link_libraries(${CERES_LIBRARIES})

    add_executable(ceres_test
            test_derivative.cpp
            test_powell.cpp
            test_curve_fitting.cpp
            test_ba.cpp
            test_rosenbrock.cpp
            test_transform.cpp)
    target_link_libraries(ceres_test ${CERES_LIBRARIES})
endif()