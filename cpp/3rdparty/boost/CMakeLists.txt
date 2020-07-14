
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system filesystem regex thread)
if (Boost_FOUND)
    message(STATUS "Boost Version : ${Boost_VERSION_STRING}")

    include_directories(${Boost_INCLUDE_DIRS})

    add_executable(test_boost test_any.cpp test_barrier.cpp)

    target_link_libraries(test_boost ${Boost_LIBRARIES})
endif()