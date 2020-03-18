
add_library(static_shared_lib SHARED static_shared_lib.cpp)
target_link_libraries(static_shared_lib
        ${CMAKE_SOURCE_DIR}/bin/libstatic_lib.a)