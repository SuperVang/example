cmake_minimum_required(VERSION 3.14)
project(pc2grid)

set(CMAKE_CXX_STANDARD 14)

find_package(PCL HINTS /usr/lib/x86_64-linux-gnu/cmake)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
link_libraries(${PCL_LIBRARIES})


find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
link_libraries(${OpenCV_LIBS})

if (${OpenCV_FOUND} AND ${PCL_FOUND})

    add_library(${PROJECT_NAME} Grid.cpp)

    add_executable(occupy_grid occupy_grid.cpp)
    target_link_libraries(occupy_grid ${PROJECT_NAME} ${PCL_LIBRARIES} ${OpenCV_LIBS})

    add_executable(split_img split_img.cpp)
    target_link_libraries(split_img ${OpenCV_LIBS})

endif()