cmake_minimum_required (VERSION 3.16)

project (test_yaml)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 11)


include_directories(${PROJECT_SOURCE_DIR}/include)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(SRC src/load_yaml.cpp)

# generate lib
add_library(load_yaml SHARED ${SRC})


# include_directories(${PROJECT_SOURCE_DIR}/../add_library/include)

set(SRC test_yaml.cpp)
add_executable(use_lib ${SRC})

target_link_libraries(use_lib
    ${PROJECT_SOURCE_DIR}/lib/libload_yaml.so
)
