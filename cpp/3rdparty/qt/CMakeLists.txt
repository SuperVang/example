
find_package(Qt5 COMPONENTS Widgets REQUIRED)
if(Qt5_FOUND)
    message(STATUS "Qt5 Found")

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)

    add_executable(simple_window simple_window.cpp
#            mainwindow.ui
#            manwindow.cpp
#            resources.qrc
            )
    target_link_libraries(simple_window Qt5::Widgets)
endif()
