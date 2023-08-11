project(main)

add_subdirectory(MyLib1)
add_subdirectory(MyLib2)
add_subdirectory(PreLoad)

add_executable(main main.cpp)

# link squence
target_link_libraries(main MyLib1 MyLib2)
# target_link_libraries(main MyLib2 MyLib1)