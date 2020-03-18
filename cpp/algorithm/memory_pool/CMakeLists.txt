

set(test_file cxx_alloc_example.cpp malloc_free_example.cpp)


set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS filesystem regex)
if (Boost_FOUND)
    message(STATUS "Boost Version : ${Boost_VERSION_STRING}")

    include_directories(${Boost_INCLUDE_DIRS})

    list(APPEND test_file boost_pool_example.cpp)

endif()

add_executable(memory_pool_test ${test_file})

if (Boost_FOUND)
    target_link_libraries(memory_pool_test ${Boost_LIBRARIES})
endif()

