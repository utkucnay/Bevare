cmake_minimum_required(VERSION 3.6)

set(ProjectName Bevare)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

project(${ProjectName})

option(BEVARE_TEST "Enable All Tests" ON)

add_subdirectory(Bevare)

add_subdirectory(BevareEditor)

add_subdirectory(BevareSandbox)
