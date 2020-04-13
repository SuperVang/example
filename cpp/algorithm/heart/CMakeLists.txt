
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(heart_libs
    "Heart.h"
    "Heart.cpp"
    "Registry.h"
    "Registry.cpp"
    )

find_package(Boost)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)
find_package(Protobuf)
find_package(gRPC)

find_package(fmt REQUIRED)

if (Boost_FOUND AND Protobuf_FOUND AND gRPC_FOUND)

  include_directories(${Boost_INCLUDE_DIRS})
  include_directories(${Protobuf_INCLUDE_DIRS})

  include(protoc)
  protoc(DIRECTORIES "proto" OUTPUT_DIR PROTO_GEN_DIR GRPC INCLUDES "proto")

  include_directories(${PROTO_GEN_DIR})

  FILE(GLOB_RECURSE PROTO_GEN_CC "${PROTO_GEN_DIR}/*.cc")

  add_library(aai_heart STATIC ${heart_libs} ${PROTO_GEN_CC})

  target_link_libraries(aai_heart gRPC::grpc++ gRPC::grpc protobuf::libprotobuf ${Boost_LIBRARIES} fmt::fmt)

endif()


