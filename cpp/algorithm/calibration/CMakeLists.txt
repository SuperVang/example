
include_directories("/usr/include/eigen3/")

add_executable(calibration_test
        handeye_test.cpp
        handeye/navy_calibration.h
        handeye/navy_calibration.cpp
        handeye/tsai_calibration.h
        handeye/tsai_calibration.cpp)