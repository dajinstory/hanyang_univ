cmake_minimum_required(VERSION 3.5)
project(hw1)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/machar.c src/machar_double.c src/my.c src/main.c)

include_directories(. src)

ADD_EXECUTABLE(hw1 ${SOURCE_FILES})
