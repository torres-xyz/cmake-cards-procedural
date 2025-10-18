# Using min version 3.26 for "copy_directory_if_different" command
cmake_minimum_required(VERSION 3.26)
project(cmake-cards-procedural LANGUAGES CXX)

include(CTest)
add_subdirectory(tests)

add_subdirectory(vendors)
add_subdirectory(src)
