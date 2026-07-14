cmake_minimum_required(VERSION 3.15)
project(Evolution)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /WX) 
else()
    add_compile_options(-Wall -Wextra -Wpedantic) 
endif()

include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})