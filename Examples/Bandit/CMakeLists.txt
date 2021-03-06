cmake_minimum_required(VERSION 3.21)

project(SkySpec_Example_Bandit VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkySpec_Example_Bandit_Files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(SkySpec_Example_Bandit SHARED ${SkySpec_Example_Bandit_Files})
target_include_directories(SkySpec_Example_Bandit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(SkySpec_Example_Bandit PRIVATE SkySpecServer)
target_include_directories(SkySpec_Example_Bandit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../SkySpec.Server/include) # <--- FIXME
target_include_directories(SkySpec_Example_Bandit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SkySpec.Papyrus/include) # <-- FIXME

find_package(CommonLibSSE CONFIG REQUIRED)
target_link_libraries(SkySpec_Example_Bandit PRIVATE CommonLibSSE::CommonLibSSE)

# Hard code temporarily
set(DLL_FOLDER "C:/Users/mrowr/Dropbox/Skyrim/Mod Authoring/Mods/Mrowr Purr's SKSE Plugins - AE/SKSE/Plugins")

add_custom_command(
	TARGET "${PROJECT_NAME}"
	POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}"
	VERBATIM
)

add_executable(RunBanditTests ${CMAKE_CURRENT_SOURCE_DIR}/RunSpecs.cpp)
target_include_directories(RunBanditTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../SkySpec.Runner/include) # <--- FIXME

# Needed until we make it a linkable library (?)
find_package(websocketpp CONFIG REQUIRED)
target_link_libraries(RunBanditTests PRIVATE websocketpp::websocketpp)
find_package(Boost REQUIRED COMPONENTS random)
target_link_libraries(RunBanditTests PRIVATE Boost::boost Boost::random)
