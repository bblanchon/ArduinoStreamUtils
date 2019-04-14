# StreamUtils - github.com/bblanchon/ArduinoStreamUtils
# Copyright Benoit Blanchon 2019
# MIT License

cmake_minimum_required(VERSION 3.0)
project(StreamUtils)

enable_testing()

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
add_subdirectory(test)
