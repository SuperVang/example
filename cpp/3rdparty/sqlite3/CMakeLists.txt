
find_package(SQLite3 REQUIRED)
if(SQLITE3_FOUND)
    message(STATUS "SQLite3 Inc : ${SQLITE3_INCLUDE_DIR}")
    message(STATUS "SQLite3 Lib : ${SQLITE3_LIBRARIES}")
    include_directories(${SQLITE3_INCLUDE_DIR})

    add_executable(sqlite3_test example.cpp)
    target_link_libraries(sqlite3_test ${SQLITE3_LIBRARIES})
endif()

