
find_package(libmongocxx QUIET)
if(${LIBMONGOCXX_FOUND})
    message(STATUS "MongoDB Inc : ${LIBMONGOCXX_INCLUDE_DIRS}")
    message(STATUS "MongoDB Lib : ${LIBMONGOCXX_LIBRARIES}")
    include_directories(${LIBMONGOCXX_INCLUDE_DIRS})

    add_executable(mongodb_test
        client_example.cpp
        thread_example.cpp
        bson_example.cpp)
    target_link_libraries(mongodb_test ${LIBMONGOCXX_LIBRARIES})
endif()