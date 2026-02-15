cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(beautiful-thirteen CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -Wall -Werror -Wextra -pedantic")

set(SOURCE_FILES  src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE src)