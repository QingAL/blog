project(main)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -pthread")

add_executable(main main.cpp)