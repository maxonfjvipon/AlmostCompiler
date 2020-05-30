cmake_minimum_required(VERSION 3.15)
project(LEXER)

set(CMAKE_CXX_STANDARD 14)

add_executable(LEXER main.cpp Lexer.cpp Lexer.h Node.cpp Node.h Parser.cpp Parser.h lexemtype.h nodekind.h Compiler.cpp Compiler.h)