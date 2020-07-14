
find_path(CEREAL_INCLUDE_DIR cereal/cereal.hpp
        /usr/include
        /usr/local/include)

if(IS_DIRECTORY ${CEREAL_INCLUDE_DIR})
    include_directories(${CEREAL_INCLUDE_DIR})
    add_executable(cereal_test test_xml.cpp test_binary.cpp test_json.cpp)
endif()