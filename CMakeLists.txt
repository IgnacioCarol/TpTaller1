cmake_minimum_required(VERSION 3.0)
project(TpTaller1)
find_package(SDL2 REQUIRED)
set(CMAKE_CXX_STANDARD 14)
include_directories(.)

add_executable(TpTaller1
        sdlDany.cpp
        sdlDany.h)
target_link_libraries(TpTaller1 ${SDL2_LIBRARIES})