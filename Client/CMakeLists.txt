cmake_minimum_required(VERSION 3.10)

project(GuessingGameClient)

set (CMAKE_CXX_STANDARD 11)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(GuessingGameClient ${SOURCES})
