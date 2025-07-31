cmake_minimum_required(VERSION 3.10)
project(log_plugin C)

set(CMAKE_C_STANDARD 99)

# Path to your mosquitto source
set(MOSQUITTO_DIR ../mosquitto)

# Include Mosquitto broker + client headers
include_directories(${MOSQUITTO_DIR}/lib)
include_directories(${MOSQUITTO_DIR}/include)
include_directories(${MOSQUITTO_DIR}/src)

# Build the plugin
add_library(log_plugin MODULE log.c)

# Remove 'lib' prefix, ensure .so suffix
set_target_properties(log_plugin PROPERTIES PREFIX "" SUFFIX ".so")
