cmake_minimum_required(VERSION 3.10)
project(ffs_test)

include_directories("../")

add_executable(${PROJECT_NAME} test.cpp)

target_link_libraries(${PROJECT_NAME} ffilesys)