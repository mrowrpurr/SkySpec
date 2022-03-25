cmake_minimum_required(VERSION 3.21)

project(SkySpecRunner VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkySpecRunner_Files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE SkySpecRunner_Headers ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SkySpecRunner_Cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(SkySpecRunner INTERFACE ${SkySpecRunner_Files})
target_include_directories(SkySpecRunner INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
