cmake_minimum_required(VERSION 3.20)

project(
  "JobySim"
  VERSION 0.0.1
  LANGUAGES CXX)

set(${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS} -Wall)

add_subdirectory(src)
