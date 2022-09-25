cmake_minimum_required(VERSION 3.21)

# Set the project name and version
project(netlink-socket-demo VERSION 1.0)

add_library(
  conv_pid STATIC
  conv_pid.c
)

add_executable(
  recv
  recv.c
)
target_link_libraries(
  recv
  conv_pid
)

add_executable(
  send
  send.c
)
target_link_libraries(
  send
  conv_pid
)
