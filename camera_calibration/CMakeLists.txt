cmake_minimum_required(VERSION 2.6)

project(opencv_camera_calibration)

find_package(OpenCV 3 REQUIRED)

add_executable(opencv_camera_calibration 
                calibration.cpp)
            
target_link_libraries(opencv_camera_calibration
                     ${OpenCV_LIBS})
