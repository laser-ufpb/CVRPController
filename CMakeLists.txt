cmake_minimum_required(VERSION 3.0)

set(this CVRPController)

project(${this})

set(CMAKE_CXX_STANDARD 11)
set(${this} C CXX)

add_subdirectory(src)

add_executable(${this} src/main.cpp)
add_executable(DummySolver DummySolver.cpp)
enable_testing()
add_test(NAME DummySolver COMMAND ${this} Dummy ../InstancesRounded/X-n101-k25.vrp 1 2000 60 27591 1 DummySolver)

target_link_libraries(${this} PUBLIC
    Controller   
    Data
    OutputFile
    iSolution
    dSolution
    Instance
)

target_include_directories(${this} PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
)
