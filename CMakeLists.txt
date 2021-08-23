cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial)

set (CMAKE_CXX_STANDARD 11)
# add the executable
add_executable(ctorv ctorv.cpp Lexer.cpp Parser.cpp AST.cpp RISCV.cpp CompileUnit.cpp)
