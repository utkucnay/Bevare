cmake_minimum_required(VERSION 3.6)

set(ProjectName Bevare)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

project(${ProjectName})

option(BEVARE_TEST "Enable All Tests" ON)

if (BEVARE_TEST)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(Bevare)

add_subdirectory(BevareEditor)

add_subdirectory(BevareSandbox)
