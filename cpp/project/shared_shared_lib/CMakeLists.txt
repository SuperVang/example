
add_library(shared_shared_lib SHARED shared_shared_lib.cpp)

target_link_libraries(shared_shared_lib
        ${CMAKE_SOURCE_DIR}/bin/libshared_lib.so)