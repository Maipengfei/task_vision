cmake_minimum_required(VERSION 2.8)
project(armDetection2)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})
add_executable( armDetection2 armDetection2.cpp)
target_link_libraries( armDetection2 ${OpenCV_LIBS} )
