
find_package(Protobuf)
if(Protobuf_FOUND)
    message(STATUS "Protobuf Version : ${Protobuf_VERSION}")
    message(STATUS "Protobuf Inc : ${PROTOBUF_INCLUDE_DIRS}")
    message(STATUS "Protobuf Lib : ${PROTOBUF_LIBRARIES}")
    include_directories(${PROTOBUF_INCLUDE_DIRS})

    set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
    set(PROTO_FILES type/SearchRequest.proto type/Test.proto)
    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
    list(APPEND PROTO_FILES ${PROTO_SRCS} ${PROTO_HDRS})
    message(STATUS "Protobuf File : ${PROTO_FILES}")

    add_executable(protobuf_test example.cpp ${PROTO_FILES})
    target_link_libraries(protobuf_test ${PROTOBUF_LIBRARIES})
endif()