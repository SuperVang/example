
find_package(PROJ4 REQUIRED)
include_directories(${PROJ4_INCLUDE_DIRS})

add_executable(proj4_test test_proj4.cpp)
target_link_libraries(proj4_test ${PROJ4_LIBRARIES})


