cmake_minimum_required(VERSION 3.23)
project(http_protocol_cpp)

set(CMAKE_CXX_STANDARD 14)

add_library(util util.cpp)
target_include_directories(util PUBLIC include/net)

add_library(
        net_socket
        net_socket/inet_server_socket.cpp
        net_socket/inet_socket.cpp
        net_socket/ipv4_address.cpp
        net_socket/sock_address.cpp
        net_socket/socket_exception.cpp
)
target_include_directories(net_socket PUBLIC  include/net)
target_link_libraries(net_socket util)

add_library(
        http
        http/header.cpp
        http/http_exception.cpp
        http/request.cpp
        http/response.cpp
        http/http_server.cpp
        http/http_client.cpp
)
target_include_directories(http PUBLIC include/net)
target_link_libraries(http net_socket util)
