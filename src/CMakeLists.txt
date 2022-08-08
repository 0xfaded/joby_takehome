set(JOBYSIM_SRCS
  "sim_builder.cpp"
  "sim_executor.cpp"

  "actors/aircraft.cpp"
  "actors/environment.cpp"
  "actors/fleet_obsever.cpp"
  "actors/root_actor.cpp"

  "aircraft/spec.cpp")

# libjobysim
add_library(jobysim SHARED ${JOBYSIM_SRCS})
target_include_directories(jobysim PUBLIC "${CMAKE_SOURCE_DIR}/include")

# driver executable
add_executable(jobysim_driver "main.cpp")
target_link_libraries(jobysim_driver jobysim)
set_target_properties(jobysim_driver PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


# tests
include(GoogleTest)

function(JOBY_TEST NAME)
  add_executable(${NAME}_test ${NAME}_test.cpp)
  target_link_libraries(${NAME}_test PRIVATE jobysim GTest::gtest_main)
  gtest_discover_tests(${NAME}_test)
endfunction()

add_subdirectory(actors)

joby_test(integration)
