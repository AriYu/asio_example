cmake_minimum_required(VERSION 2.8)
project( asio_chat )

ADD_DEFINITIONS(
-g
-O0
-Wall
-std=c++11
)

FIND_PACKAGE( Boost 1.53 COMPONENTS system REQUIRED) 

include_directories(${Boost_INCLUDE_DIRS}) 
add_executable(chat_server chat_server.cpp) 
target_link_libraries(chat_server ${Boost_LIBRARIES} pthread )

include_directories(${Boost_INCLUDE_DIRS}) 
add_executable(chat_client chat_client.cpp) 
target_link_libraries(chat_client ${Boost_LIBRARIES} pthread )
