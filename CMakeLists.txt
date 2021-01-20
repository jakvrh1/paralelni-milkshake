cmake_minimum_required(VERSION 3.10)

project(paralelni-milkshake)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-O2 -g)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(cpp/libs)
include_directories(cpp)

set(SEQUENTIAL_SOURCES cpp/sequential/sequential.cpp)
set(PIPELINE_SOURCES cpp/pipeline/pipeline.cpp)
set(PIPESQUARE_SOURCES cpp/pipesquare/pipesquare.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../cpp/sequential)
add_executable(sequential ${SEQUENTIAL_SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../cpp/pipeline)
add_executable(pipeline ${PIPELINE_SOURCES})
target_link_libraries(pipeline PRIVATE Threads::Threads)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../cpp/pipesquare)
add_executable(pipesquare ${PIPESQUARE_SOURCES})
target_link_libraries(pipesquare PRIVATE Threads::Threads)

add_custom_target(seq DEPENDS)
add_custom_target(pip DEPENDS)
add_custom_target(sqr DEPENDS)

add_custom_command(
  TARGET pip 
  COMMAND "bash"
  ARGS "run.sh"
  WORKING_DIRECTORY "../cpp/pipeline"
  )

add_custom_command(
  TARGET sqr 
  COMMAND "bash"
  ARGS "run.sh"
  WORKING_DIRECTORY "../cpp/pipesquare"
  )

add_custom_command(
  TARGET seq
  COMMAND "bash"
  ARGS "run.sh"
  WORKING_DIRECTORY "../cpp/sequential"
  )
