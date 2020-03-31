cmake_minimum_required(VERSION 3.5)
project(example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
message(STATUS "CMAKE MODULE PATH : ${CMAKE_MODULE_PATH}")

message(STATUS "CMAKE MINOR VERSION : ${CMAKE_MINOR_VERSION}")
if(${CMAKE_MINOR_VERSION} LESS 12)
    find_package(PythonLibs 3 REQUIRED)
    find_package(PythonInterp 3 REQUIRED)
else()
    find_package(Python3 REQUIRED)
endif()

set(MYSQLCPPCONN_ROOT_DIR "/usr/;/usr/include/" CACHE PATH "")
message(STATUS "MySQL Root : ${MYSQLCPPCONN_ROOT_DIR}")
find_path(MYSQLCPPCONN_INCLUDE_DIR cppconn/driver.h PATHS ${MYSQLCPPCONN_ROOT_DIR})
message(STATUS "MySQL cpp conn inc : ${MYSQLCPPCONN_INCLUDE_DIR}")

enable_testing()
find_package(GTest REQUIRED)
message(STATUS "GTest Inc : ${GTEST_INCLUDE_DIRS}")
message(STATUS "GTest Lib : ${GTEST_BOTH_LIBRARIES}")
include_directories(${GTEST_INCLUDE_DIRS})
link_libraries(${GTEST_BOTH_LIBRARIES})

# # Download and unpack googletest at configure time
# configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
# execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
# if(result)
#   message(FATAL_ERROR "CMake step for googletest failed: ${result}")
# endif()
# execute_process(COMMAND ${CMAKE_COMMAND} --build .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
# if(result)
#   message(FATAL_ERROR "Build step for googletest failed: ${result}")
# endif()

# # Add googletest directly to our build. This defines
# # the gtest and gtest_main targets.
# add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
#                  ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
#                  EXCLUDE_FROM_ALL)

# # The gtest/gtest_main targets carry header search path
# # dependencies automatically when using CMake 2.8.11 or
# # later. Otherwise we have to add them here ourselves.
# if (CMAKE_VERSION VERSION_LESS 2.8.11)
#   include_directories("${gtest_SOURCE_DIR}/include")
# endif()

# link_libraries(gtest gtest_main)

add_subdirectory(basic)
add_subdirectory(algorithm)
add_subdirectory(3rdparty)


add_subdirectory(project)
