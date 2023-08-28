cmake_minimum_required (VERSION 3.16)

project (test_disjoint_set)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

SET(EIGEN_INCLUDE_DIR "/usr/include/eigen3")
include_directories(${EIGEN_INCLUDE_DIR}
                    ${OpenCV_INCLUDE_DIRS})

add_executable(test_disjoint_set 
                test_disjoint_set.cpp
                )

target_link_libraries(test_disjoint_set 
                      ${OpenCV_LIBS})
