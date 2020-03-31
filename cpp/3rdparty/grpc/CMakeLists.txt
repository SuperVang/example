cmake_minimum_required(VERSION 3.12)

project(proto_rpc)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using Protobuf : ${protobuf_VERSION}")

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC : ${gRPC_VERSION}")

#find_package(OpenCV REQUIRED)


file(GLOB proto_file "proto/*.cc" "proto/*.h")
message(STATUS "proto file : ${proto_file}")

add_executable(service service.cpp ${proto_file})
target_link_libraries(service gRPC::grpc++_unsecure protobuf::libprotobuf)

add_executable(client client.cpp ${proto_file})
target_link_libraries(client gRPC::grpc++_unsecure protobuf::libprotobuf)

#add_executable(stream_service stream_service.cpp ${proto_file})
#target_link_libraries(client gRPC::grpc++_unsecure protobuf::libprotobuf)

#add_executable(stream_client stream_client.cpp ${proto_file})
#target_link_libraries(client gRPC::grpc++_unsecure protobuf::libprotobuf)

