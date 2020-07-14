
find_package(PugiXML REQUIRED)

include_directories(${PUGIXML_INCLUDE_DIR})
add_executable(test_pugixml test_loader.cpp test_accessor.cpp)
target_link_libraries(test_pugixml ${PUGIXML_LIBRARY})

