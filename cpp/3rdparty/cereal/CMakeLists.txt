
find_path(CEREAL_INCLUDE_DIR cereal/cereal.hpp
        /usr/include
        /usr/local/include)

if(IS_DIRECTORY ${CEREAL_INCLUDE_DIR})
    include_directories(${CEREAL_INCLUDE_DIR})
    add_executable(test_cereal test_xml.cpp)
endif()