cmake_minimum_required(VERSION 3.21)

project(SkySpecServer VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkySpecServer_Files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(SkySpecServer SHARED ${SkySpecServer_Files})
target_include_directories(SkySpecServer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(SkySpecServer PROPERTIES OUTPUT_NAME "!!SkySpec.Server")

find_package(websocketpp CONFIG REQUIRED)
target_link_libraries(SkySpecServer PRIVATE websocketpp::websocketpp)

find_package(CommonLibSSE CONFIG REQUIRED)
target_link_libraries(SkySpecServer PRIVATE CommonLibSSE::CommonLibSSE)

# Hard code temporarily
set(DLL_FOLDER "C:/Users/mrowr/Dropbox/Skyrim/Mod Authoring/Mods/Mrowr Purr's SKSE Plugins - AE/SKSE/Plugins")

add_custom_command(
	TARGET "${PROJECT_NAME}"
	POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}"
	VERBATIM
)
