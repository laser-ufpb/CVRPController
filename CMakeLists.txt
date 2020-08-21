cmake_minimum_required(VERSION 3.0)

set(this main)

project(${this})

set(CMAKE_CXX_STANDARD 11)
set(${this} C CXX)

add_subdirectory(src)

add_executable(${this} src/main.cpp)

target_link_libraries(${this} PUBLIC
    Data
    Controller   
    OutputFile
    iSolution
    dSolution
    Instance
    Solution 
)

target_include_directories(${this} PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
)