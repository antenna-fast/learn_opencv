cmake_minimum_required (VERSION 3.16)

project (test_point)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV 4 REQUIRED)

SET(EIGEN_INCLUDE_DIR "/usr/include/eigen3")
include_directories(${EIGEN_INCLUDE_DIR}
                    ${OpenCV_INCLUDE_DIRS})

add_executable(test_point test_point.cpp)
target_link_libraries(test_point 
                      ${OpenCV_LIBS})