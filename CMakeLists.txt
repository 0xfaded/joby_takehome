cmake_minimum_required(VERSION 3.20)

project(
  "JobySim"
  VERSION 0.0.1
  LANGUAGES CXX)

set(${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS} -Wall)

# Suggested method of interfacing with gtest according to:
# https://google.github.io/googletest/quickstart-cmake.html
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# add subdirectories
add_subdirectory(src)
