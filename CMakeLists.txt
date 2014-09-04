cmake_minimum_required (VERSION 2.8)
project (Compilador)

FILE (GLOB_RECURSE C_SOURCES "src/*.c")
INCLUDE_DIRECTORIES ("includes")

ADD_EXECUTABLE (compiler ${C_SOURCES})
