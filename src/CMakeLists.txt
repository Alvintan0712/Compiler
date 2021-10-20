cmake_minimum_required(VERSION 3.0)
project(src)

set(CMAKE_CXX_STANDARD 11)

add_executable(src main.cpp LexicalAnalyzer.cpp LexicalAnalyzer.h Symbol.h GrammarAnalyzer.cpp GrammarAnalyzer.h Symbol.cpp ErrorHandling.cpp ErrorHandling.h Ast.cpp Ast.h)
