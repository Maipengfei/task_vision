cmake_minimum_required(VERSION 2.8)

project(fanDetection)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})
add_executable( fanDetection fanDetection.cpp)
target_link_libraries( fanDetection ${OpenCV_LIBS} )
