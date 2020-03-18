
find_package(OpenCV)
find_package(GDAL)
if(GDAL_FOUND AND OpenCV_FOUND)
    message(STATUS "GDAL INC : ${GDAL_INCLUDE_DIR}")
    message(STATUS "GDAL LIB : ${GDAL_LIBRARY}")
    include_directories(${GDAL_INCLUDE_DIR})

    message(STATUS "OpenCV Inc : ${OpenCV_INCLUDE_DIRS}")
    message(STATUS "OpenCV Lib : ${OpenCV_LIBS}")
    include_directories(${OpenCV_INCLUDE_DIRS})

    add_executable(geometry_test polygon_union.cpp)
    target_link_libraries(geometry_test ${GDAL_LIBRARY} ${OpenCV_LIBS})
endif()