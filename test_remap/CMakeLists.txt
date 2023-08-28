cmake_minimum_required (VERSION 3.16)

project (test_remap)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV 4 REQUIRED)

SET(EIGEN_INCLUDE_DIR "/usr/include/eigen3")
include_directories(${EIGEN_INCLUDE_DIR}
                    ${OpenCV_INCLUDE_DIRS})

add_executable(test_remap 
                # test_remap.cpp
                test_remap_project.cpp
                )

target_link_libraries(test_remap 
                ${OpenCV_LIBS}
                )
