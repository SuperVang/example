function (protoc)
    # Protobuf
    cmake_parse_arguments (ARG "" "OUTPUT_DIR;GRPC_BIN;PROTOC_BIN" "DIRECTORIES;INCLUDES" ${ARGN})

    if (NOT DEFINED ARG_DIRECTORIES)
        message (FATAL_ERROR "The argument DIRECTORIES is required!")
    endif ()

    if (NOT DEFINED ARG_OUTPUT_DIR)
        message (FATAL_ERROR "The argument OUTPUT_DIR is required!")
    endif ()

    if (DEFINED ARG_INCLUDES)
        set (_protobuf_include_path "")
        foreach(_path ${ARG_INCLUDES})
            get_filename_component (_abs_path ${_path} ABSOLUTE)
            list (FIND _protobuf_include_path ${_abs_path} _contains_already)
            if (${_contains_already} EQUAL -1)
                          list (APPEND _protobuf_include_path ${_abs_path})
            endif ()
        endforeach()
    endif ()

    set (_PROTO_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/proto")
    if (NOT EXISTS ${_PROTO_OUT_DIR})
        execute_process (
            COMMAND ${CMAKE_COMMAND} -E make_directory ${_PROTO_OUT_DIR}
            OUTPUT_QUIET)
    endif ()

    if (DEFINED ARG_PROTOC_BIN)
        set(PROTOC_EXE "${ARG_PROTOC_BIN}")
    else()
        set(PROTOC_EXE "protoc")
    endif ()

    if (DEFINED ARG_GRPC_BIN)
        set (GRPC_PARAM "--grpc_out ${_PROTO_OUT_DIR} --plugin=protoc-gen-grpc=${ARG_GRPC_BIN}")
    endif ()

    foreach (_dir ${ARG_DIRECTORIES})
        file (GLOB_RECURSE ALL_FILES ${_dir}/*.proto)

        foreach (_proto ${ALL_FILES})
            get_filename_component (_abs_file ${_proto} ABSOLUTE)
            get_filename_component (_abs_dir ${_abs_file} DIRECTORY)

            list (FIND _protobuf_include_path ${_abs_dir} _contains_already)
            if (${_contains_already} EQUAL -1)
                list (APPEND _protobuf_include_path ${_abs_dir})
            endif ()

            exec_program (${PROTOC_EXE}
                ARGS ${GRPC_PARAM} --cpp_out ${_PROTO_OUT_DIR} -I ${_protobuf_include_path} ${_abs_file}
                OUTPUT_VARIABLE STDOUT_OUTPUT
                RETURN_VALUE _RET)

            if (NOT ("${_RET}" STREQUAL "0"))
                message (FATAL_ERROR "Failed to generate proto, ${STDOUT_OUTPUT}")
            endif ()

            # debug_info(${STDOUT_OUTPUT})
        endforeach ()
    endforeach ()

    set (${ARG_OUTPUT_DIR} ${_PROTO_OUT_DIR} PARENT_SCOPE)

endfunction ()