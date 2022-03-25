cmake_minimum_required(VERSION 3.21)

project(SkySpec VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkySpec_Files ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(SkySpec INTERFACE ${SkySpec_Files})
target_include_directories(SkySpec INTERFACE ${CMAKE_CURRENT_SOURCE_DIR/include})
