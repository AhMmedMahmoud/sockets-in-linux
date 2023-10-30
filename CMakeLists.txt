cmake_minimum_required(VERSION 3.14)

project(Async_BSD_Socket_Lib)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_library(
  async_bsd_socket_lib
  src/network_socket.cpp
  src/tcp_client.cpp
  src/tcp_listener.cpp
)

target_include_directories(async_bsd_socket_lib
  INTERFACE include
)


########### server ##########
add_executable(client
  samples/demo_client.cpp
)


target_link_libraries(client
  pthread
  async_bsd_socket_lib
)


########### server ##########
add_executable(server
  samples/demo_server.cpp
)


target_link_libraries(server
  pthread
  async_bsd_socket_lib
)