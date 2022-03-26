cmake_minimum_required(VERSION 3.21)

project(SkySpec_Example_Papyrus VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkySpec_Example_Papyrus_Files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(RunPapyrusTests ${CMAKE_CURRENT_SOURCE_DIR}/RunSpecs.cpp)
target_include_directories(RunPapyrusTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../SkySpec.Runner/include) # <--- FIXME
target_include_directories(RunPapyrusTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../SkySpec.Papyrus/include) # <--- FIXME
target_include_directories(RunPapyrusTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../Papyrus.Reflection/include) # <--- FIXME

# Needed until we make it a linkable library (?)
find_package(websocketpp CONFIG REQUIRED)
target_link_libraries(RunPapyrusTests PRIVATE websocketpp::websocketpp)
find_package(Boost REQUIRED COMPONENTS random)
target_link_libraries(RunPapyrusTests PRIVATE Boost::boost Boost::random)
