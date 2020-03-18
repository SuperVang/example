add_subdirectory(static_lib)
add_subdirectory(static_static_lib)
add_subdirectory(shared_lib)
add_subdirectory(shared_shared_lib)
add_subdirectory(static_shared_lib)
add_subdirectory(app)


set(books_version 1.1.1)
message(STATUS "Books Version : ${books_version}")
if (books_version VERSION_GREATER_EQUAL 1.1.1)
    message(STATUS "Books Version ${books_version} Greater Equal 1.1.1")
endif()