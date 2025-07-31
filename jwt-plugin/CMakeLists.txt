cmake_minimum_required(VERSION 3.10)
project(jwt_auth C)

# Force Mosquitto 2.x headers location if needed
include_directories(/usr/local/include)

add_library(jwt_auth MODULE jwt_auth.c)
set_target_properties(jwt_auth PROPERTIES PREFIX "" SUFFIX ".so")
