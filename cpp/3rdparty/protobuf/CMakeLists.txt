
find_package(Protobuf QUIET)
if(Protobuf_FOUND)

    include(protoc)
    protoc(DIRECTORIES "proto" OUTPUT_DIR PROTO_GEN_DIR INCLUDES "proto")
    include_directories(${PROTO_GEN_DIR})
    file(GLOB_RECURSE PROTO_FILES "${PROTO_GEN_DIR}/*.cc")

    add_executable(protobuf_test example.cpp ${PROTO_FILES})
    target_link_libraries(protobuf_test ${PROTOBUF_LIBRARIES})
endif()