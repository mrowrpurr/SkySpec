cmake_minimum_required(VERSION 3.21)

project(SkySpecRunner VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkySpecRunner_Files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(SkySpecRunner INTERFACE ${SkySpecRunner_Files})
target_include_directories(SkySpecRunner INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(SkySpecRunner INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../SkySpec.Papyrus/include) # <-- FIXME
target_include_directories(SkySpecRunner INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../Papyrus.Reflection/include) # <-- FIXME
find_package(websocketpp CONFIG REQUIRED)
target_link_libraries(SkySpecRunner INTERFACE websocketpp::websocketpp)
find_package(Boost REQUIRED COMPONENTS random)
target_link_libraries(SkySpecRunner INTERFACE Boost::boost Boost::random)
