cmake_minimum_required(VERSION 3.14)

project(POSIXNetworkSocketLib)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_library(
  POSIXNetworkSocketLib
  src/network_socket.cpp
  src/TCPClient.cpp
  src/TCPServer.cpp
  src/UDPClientServer.cpp
  src/poller.cpp

  src/TCPServerMultipleClient.cpp
)

target_include_directories(POSIXNetworkSocketLib  
  INTERFACE include
)



########### client TCP ##########
add_executable(client_tcp
  samples/demo_TCPClient.cpp
)

target_link_libraries(client_tcp
  pthread
  POSIXNetworkSocketLib
)


########### server TCP ##########
add_executable(server_tcp
  samples/demo_TCPServer.cpp
)

target_link_libraries(server_tcp
  pthread
  POSIXNetworkSocketLib
)



########### client UDp ##########
add_executable(client_udp
  samples/demo_UDPClient.cpp
)

target_link_libraries(client_udp
  pthread
  POSIXNetworkSocketLib
)


########### server UDp ##########
add_executable(server_udp
  samples/demo_UDPServer.cpp
)

target_link_libraries(server_udp
  pthread
  POSIXNetworkSocketLib
)


########### TCPServerPoller ##########
add_executable(TCPServerPoller
  samples/demo_TcpServerPoller.cpp
)

target_link_libraries(TCPServerPoller
  pthread
  POSIXNetworkSocketLib
)


########### TCPCLientPoller ##########
add_executable(TCPCLientPoller
samples/demo_TCPClientPoller.cpp
)

target_link_libraries(TCPCLientPoller
  pthread
  POSIXNetworkSocketLib
)



########### UDPServerPoller ##########
add_executable(UDPServerPoller
  samples/demo_UDPServerPoller.cpp
)

target_link_libraries(UDPServerPoller
  pthread
  POSIXNetworkSocketLib
)


########### UDPCLientPoller ##########
add_executable(UDPClientPoller
samples/demo_UDPClientPoller.cpp
)

target_link_libraries(UDPClientPoller
  pthread
  POSIXNetworkSocketLib
)