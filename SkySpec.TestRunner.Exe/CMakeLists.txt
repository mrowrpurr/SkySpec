cmake_minimum_required(VERSION 3.21)

project(SkySpecRunnerExe VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkySpecRunnerExe_Files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(SkySpecRunnerExe ${SkySpecRunnerExe_Files})
target_include_directories(SkySpecRunnerExe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SkySpec.TestRunner/include)
