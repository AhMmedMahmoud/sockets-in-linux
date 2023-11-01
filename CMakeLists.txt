cmake_minimum_required(VERSION 3.14)

project(POSIXNetworkSocketLib)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_library(
  POSIXNetworkSocketLib
  src/network_socket.cpp
  src/tcp_client.cpp
  src/tcp_listener.cpp
)

target_include_directories(POSIXNetworkSocketLib  
  INTERFACE include
)



########### client ##########
add_executable(client
  samples/demo_client.cpp
)


target_link_libraries(client
  pthread
  POSIXNetworkSocketLib
)


########### server ##########
add_executable(server
  samples/demo_server.cpp
)


target_link_libraries(server
  pthread
  POSIXNetworkSocketLib
)