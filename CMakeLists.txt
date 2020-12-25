cmake_minimum_required(VERSION 3.15)
project(ImageProcessing)
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 11)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(ImageProcessing main.cpp BlockMatch.cpp BlockMatch.h
        CosTrans.cpp CosTrans.h Fourier.cpp Fourier.h Process.cpp Process.h
        LKtrack.cpp)

target_link_libraries(ImageProcessing ${OpenCV_LIBS})