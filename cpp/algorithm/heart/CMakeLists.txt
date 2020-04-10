
add_library(aai_heart STATIC
    "Heart.h"
    "Heart.cpp"
    "Registry.h"
    "Registry.cpp"
)

# target_link_libraries(aai_heart PUBLIC
#     CONAN_PKG::boost
#     CONAN_PKG::fmt
#     ${AAI_JSON5_PARSER_LIBRARIES}
#     CONAN_PKG::sqlitecpp
#     CONAN_PKG::flatbuffers
#     CONAN_PKG::spdlog
#     CONAN_PKG::grpc
#     CONAN_PKG::cereal
#     aai_proto_pb
#     )

target_include_directories(aai_heart ${CMAKE_CURRENT_SOURCE_DIR}/proto)
