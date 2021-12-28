cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
project(Consumer)
#find_library(OpenCV)
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
message(STATUS "start running cmake...")
find_package(Boost 1.76.0)
if(Boost_FOUND)
    message(STATUS "boost found")
    include_directories("/usr/local/Cellar/boost/1.76.0/include/boost")
endif()
add_executable(Consumer main.cpp
               udpSocket/udpSocket.cpp udpSocket/udpSocket.h
               threadpool/threadpool.cpp threadpool/threadpool.h
               udpListener/udpListener.cpp udpListener/udpListener.h
        imgProcessor/ImgProcessor.h params.h imgProcessor/ImgProcessor.cpp)
target_link_libraries(Consumer ${OpenCV_LIBS})
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(Consumer ${Boost_LIBRARIES})
