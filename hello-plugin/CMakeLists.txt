cmake_minimum_required(VERSION 3.10)
project(hello_plugin C)

set(CMAKE_C_STANDARD 99)

set(MOSQUITTO_DIR ../mosquitto)

include_directories(${MOSQUITTO_DIR}/lib)
include_directories(${MOSQUITTO_DIR}/include)
include_directories(${MOSQUITTO_DIR}/src)

add_library(hello_plugin MODULE hello.c)

set_target_properties(hello_plugin PROPERTIES PREFIX "" SUFFIX ".so")
