
find_package(Flatbuffers CONFIG PATHS /usr/local/lib/cmake/flatbuffers)
if(Flatbuffers_FOUND)
    include_directories(${FLATBUFFERS_INCLUDE_DIR})

    add_executable(test_flatbuffers test_flatbuffers.cpp)
    target_link_libraries(test_flatbuffers flatbuffers)
endif()