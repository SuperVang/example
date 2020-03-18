
add_executable(use_static_static use_static_static.cpp)
#link_directories("${CMAKE_SOURCE_DIR}/bin/")
target_link_libraries(use_static_static
        ${CMAKE_SOURCE_DIR}/bin/libstatic_lib.a
        ${CMAKE_SOURCE_DIR}/bin/libstatic_static_lib.a
        )

add_executable(use_shared_shared use_shared_shared.cpp)

target_link_libraries(use_shared_shared
        ${CMAKE_SOURCE_DIR}/bin/libshared_lib.so
        ${CMAKE_SOURCE_DIR}/bin/libshared_shared_lib.so)

add_executable(use_static_shared use_static_shared.cpp)
target_link_libraries(use_static_shared
#        ${CMAKE_SOURCE_DIR}/bin/libstatic_lib1.a
        ${CMAKE_SOURCE_DIR}/bin/libstatic_shared_lib.so)