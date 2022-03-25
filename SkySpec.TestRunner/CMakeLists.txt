cmake_minimum_required(VERSION 3.21)

project(SkySpec_Runner VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkySpec_Runner_Files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(SkySpec_Runner STATIC ${SkySpec_Runner_Files})
add_library(SkySpec::Runner ALIAS SkySpec_Runner)
target_include_directories(SkySpec_Runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
