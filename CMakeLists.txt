cmake_minimum_required(VERSION 3.21)

project(SkySpec_All VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(SkySpec)
add_subdirectory(SkySpec.Papyrus)
add_subdirectory(SkySpec.Server)
add_subdirectory(SkySpec.Runner)
add_subdirectory(SkySpec.Runner.Exe)
add_subdirectory(Examples)
add_subdirectory(Papyrus.Reflection)
