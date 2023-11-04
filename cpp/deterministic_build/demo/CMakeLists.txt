cmake_minimum_required(VERSION 3.0)

project(main)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /experimental:deterministic /Brepro")

include_directories(./MyDll)
add_executable(main main.cpp)

add_library(MyDll SHARED MyDll/MyDll.cpp)

target_link_libraries(main MyDll)

target_link_options(main PRIVATE /experimental:deterministic /Brepro /PDBALTPATH:%_PDB%)
target_link_options(MyDll PRIVATE /experimental:deterministic /Brepro /PDBALTPATH:%_PDB%)