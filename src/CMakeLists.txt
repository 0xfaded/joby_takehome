set(JOBYSIM_SRCS
  "sim_builder.cpp"

  "actors/aircraft.cpp"
  "actors/environment.cpp"
  "actors/fleet_obsever.cpp"
  "actors/root_actor.cpp"

  "aircraft/spec.cpp")

add_library(jobysim SHARED ${JOBYSIM_SRCS})

# stand alone executable, this is not a library (yet)
target_include_directories(jobysim PUBLIC "${CMAKE_SOURCE_DIR}/include")


add_executable(jobysim_driver "main.cpp")
target_link_libraries(jobysim_driver jobysim)
set_target_properties(jobysim_driver PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
