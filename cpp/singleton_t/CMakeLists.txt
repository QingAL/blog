project(main)
set(CMAKE_C_FLAGS "${CAMKE_C_FLAGS} -std=c++11 -pthread")
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread")
add_executable(main main.cpp)