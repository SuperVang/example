

add_library(static_lib STATIC static_lib.cpp)
set_property(TARGET static_lib PROPERTY POSITION_INDEPENDENT_CODE ON)