cmake_minimum_required (VERSION 3.16)

project (test_undistortation)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV 4 REQUIRED)

# yaml
find_package(yaml-cpp REQUIRED)
include_directories(${YAML_INCLUDE_DIRS})

SET(EIGEN_INCLUDE_DIR "/usr/include/eigen3")
include_directories(${EIGEN_INCLUDE_DIR}
                    ${OpenCV_INCLUDE_DIRS})

add_executable(test_undistortation 
                # test_undistortation.cpp
                test_undistortation_yaml.cpp
                )

target_link_libraries(test_undistortation 
                      ${OpenCV_LIBS}
                      ${YAML_CPP_LIBRARIES}
                      )