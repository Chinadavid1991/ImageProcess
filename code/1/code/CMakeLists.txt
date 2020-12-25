cmake_minimum_required(VERSION 3.5.1) 
set(PROJECT_NAME main) 
project(${PROJECT_NAME})
#使用C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++11" )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG " ${CMAKE_CXX_FLAGS} -g -std=c++11")
set(CMAKE_BUILD_TYPE DEBUG)
link_directories(/usr/local/lib/)

add_executable(${PROJECT_NAME}  main-004.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
libopencv_calib3d.so
libopencv_core.so
libopencv_features2d.so 
libopencv_flann.so 
libopencv_highgui.so 
libopencv_imgcodecs.so
libopencv_imgproc.so
libopencv_ml.so
libopencv_objdetect.so
libopencv_photo.so
libopencv_shape.so
libopencv_videoio.so
libopencv_video.so
libopencv_videostab.so
)
