cmake_minimum_required(VERSION 3.0)

project(main)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DR_REF=1)

include_directories(src)

add_executable(main main.cpp src/Person.cpp)

target_compile_options(main PRIVATE -fno-elide-constructors)