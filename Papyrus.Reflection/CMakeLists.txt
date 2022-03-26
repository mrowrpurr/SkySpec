cmake_minimum_required(VERSION 3.21)

project(SkyrimPapyrusReflection VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkyrimPapyrusReflection_Files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(SkyrimPapyrusReflection INTERFACE ${SkyrimPapyrusReflection_Files})
target_include_directories(SkyrimPapyrusReflection INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(CommonLibSSE CONFIG REQUIRED)
target_link_libraries(SkyrimPapyrusReflection INTERFACE CommonLibSSE::CommonLibSSE)
