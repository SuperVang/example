
find_package(GDAL)
if(GDAL_FOUND)
    message(STATUS "GDAL INC : ${GDAL_INCLUDE_DIR}")
    message(STATUS "GDAL LIB : ${GDAL_LIBRARY}")
    include_directories(${GDAL_INCLUDE_DIR})

    add_executable(gdal_test 
         ogr_spatial_reference.cpp
         ogr_coordinate_transform.cpp
         ogr_driver.cpp
         ogr_dataset.cpp
         ogr_layer.cpp
         ogr_feature.cpp
        ogr_geometry.cpp)
    target_link_libraries(gdal_test ${GDAL_LIBRARY})
endif()