cmake_minimum_required(VERSION 3.10)
project(jwt_auth C)

set(CMAKE_C_STANDARD 99)

# Paths to dependencies
set(MOSQUITTO_SRC_DIR ~/mosquitto-dev/mosquitto)
set(LIBJWT_INCLUDE_DIR /usr/local/include)   # adjust if needed
set(LIBJWT_LIB_DIR /usr/local/lib)           # adjust if needed

include_directories(
    ${MOSQUITTO_SRC_DIR}/include
    ${LIBJWT_INCLUDE_DIR}
)

link_directories(
    ${LIBJWT_LIB_DIR}
)

add_library(jwt_auth MODULE jwt_auth.c)

target_link_libraries(jwt_auth
    jwt
    jansson
    ssl
    crypto
)

set_target_properties(jwt_auth PROPERTIES
    PREFIX ""
    SUFFIX ".so"
)
