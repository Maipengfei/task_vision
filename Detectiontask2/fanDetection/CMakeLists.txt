cmake_minimum_required(VERSION 2.8)
project(fanDetection2)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})
add_executable( fanDetection2 fanDetection2.cpp)
target_link_libraries( fanDetection2 ${OpenCV_LIBS} )
