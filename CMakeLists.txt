cmake_minimum_required(VERSION 3.20)
project(plaja_logger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(plaja_logger SHARED
        PlajaLogger.cpp)

target_compile_definitions(plaja_logger PRIVATE PLAJA_LOGGER_EXPORTS)

target_include_directories(plaja_logger PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(plaja_logger PROPERTIES
        OUTPUT_NAME "plaja_logger"
)