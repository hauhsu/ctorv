cmake_minimum_required(VERSION 3.10)

# set the project name
project(ctorv)

set (CMAKE_CXX_STANDARD 11)

# Generate comiple database for YCM
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(lib)
add_subdirectory(ctorv)
add_subdirectory(test)
