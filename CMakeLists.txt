cmake_minimum_required(VERSION 3.8...3.17)

project(mat_file_writer VERSION 1.00 LANGUAGES CXX)

option(BUILD_EXAMPLE "Build example program" OFF)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(MatFileWriter::MatFileWriter ALIAS mat_file_writer)

if(${BUILD_EXAMPLE})
    add_subdirectory(example)
endif()
