cmake_minimum_required (VERSION 3.16)

project (draw_fisheye_mask)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

SET(EIGEN_INCLUDE_DIR "/usr/include/eigen3")
include_directories(${EIGEN_INCLUDE_DIR}
                    ${OpenCV_INCLUDE_DIRS})

add_executable(draw_fisheye_mask 
                draw_fisheye_mask.cpp
                )

target_link_libraries(draw_fisheye_mask 
                      ${OpenCV_LIBS})