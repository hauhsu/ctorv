cmake_minimum_required(VERSION 3.10)

# set the project name
project(ctorv)

set (CMAKE_CXX_STANDARD 11)

# Generate comiple database for YCM
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(test)
