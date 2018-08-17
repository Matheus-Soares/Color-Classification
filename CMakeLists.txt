cmake_minimum_required(VERSION 3.12)
project(Trabalhos)

set(CMAKE_CXX_STANDARD 14)

add_executable(Trabalhos main.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Trabalhos ${OpenCV_LIBS})