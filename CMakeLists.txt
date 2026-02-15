cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(goodnum13 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Werror -Wextra -pedantic")

set(SOURCE_FILES  src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})