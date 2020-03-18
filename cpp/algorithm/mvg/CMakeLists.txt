
include_directories("/usr/include/eigen3/")

set(lib_file
        mvglib/mvg_util.cpp mvglib/mvg_util.h
        mvglib/dlt.h mvglib/dlt.cpp
        mvglib/ransac.cpp mvglib/ransac.h
        mvglib/algeba.cpp mvglib/algeba.h
        mvglib/iteration.cpp mvglib/iteration.h
        mvglib/mvg_def.h mvglib/mvg_def.cpp)
set(exe_file util_test.cpp dlt_test.cpp ransac_test.cpp algeba_test.cpp mvg_test.cpp)

add_library(mvg SHARED ${lib_file})
add_executable(mvg_test ${exe_file})
target_link_libraries(mvg_test mvg)