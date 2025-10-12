cmake_minimum_required(VERSION 3.10)
project(RayTracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenMP REQUIRED)

add_executable(raytracer src/main.cpp)

target_link_libraries(raytracer ${SDL2_LIBRARIES} OpenMP::OpenMP_CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(raytracer PRIVATE -O3 -march=native)
endif()
