cmake_minimum_required(VERSION 2.8)
project( asio_exam1 )

ADD_DEFINITIONS(
-g
-O0
-Wall
-std=c++11
)

FIND_PACKAGE( Boost 1.53 COMPONENTS system REQUIRED) 

include_directories(${Boost_INCLUDE_DIRS}) 
add_executable(asio_exam1 asio_exam1.cpp) 
target_link_libraries(asio_exam1 ${Boost_LIBRARIES} pthread )
