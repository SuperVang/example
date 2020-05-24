

find_package(Threads REQUIRED)

find_package(Protobuf QUIET)
if(${Protobuf_FOUND})
    find_package(gRPC++)
    if(${gRPC_FOUND})

        include(protoc)
        protoc(DIRECTORIES "proto" OUTPUT_DIR PROTO_GEN_DIR GRPC_BIN ${_gRPC_CPP_PLUGIN_EXECUTABLE} INCLUDES "proto")

        include_directories(${PROTO_GEN_DIR})
        file(GLOB_RECURSE proto_file "${PROTO_GEN_DIR}/*.cc")

        add_executable(grpc_server server.cpp ${proto_file})
        target_link_libraries(grpc_server gRPC::grpc++_unsecure protobuf::libprotobuf)

        add_executable(grpc_client client.cpp ${proto_file})
        target_link_libraries(grpc_client gRPC::grpc++_unsecure protobuf::libprotobuf)

        #add_executable(stream_service stream_service.cpp ${proto_file})
        #target_link_libraries(client gRPC::grpc++_unsecure protobuf::libprotobuf)

        #add_executable(stream_client stream_client.cpp ${proto_file})
        #target_link_libraries(client gRPC::grpc++_unsecure protobuf::libprotobuf)
    endif()
endif()







