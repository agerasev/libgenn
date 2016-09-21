cmake_minimum_required(VERSION 2.8.11)
project(libgenn)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIBGENN ${INCLUDE} PARENT_SCOPE)
include_directories(${INCLUDE})

set(SOURCE source/network.cpp)

add_library(genn STATIC ${SOURCE})
