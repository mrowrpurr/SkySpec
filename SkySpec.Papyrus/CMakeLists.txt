cmake_minimum_required(VERSION 3.21)

project(SkySpecPapyrus VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkySpecPapyrus_Files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(SkySpecPapyrus INTERFACE ${SkySpecPapyrus_Files})
target_include_directories(SkySpecPapyrus INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(SkySpecPapyrus INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../Papyrus.Reflection/include) # <-- FIXME
find_package(CommonLibSSE CONFIG REQUIRED)
target_link_libraries(SkySpecPapyrus PRIVATE CommonLibSSE::CommonLibSSE)
