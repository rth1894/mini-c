cmake_minimum_required(VERSION 3.20)

project(mini-c)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(
    mini-c
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
)
