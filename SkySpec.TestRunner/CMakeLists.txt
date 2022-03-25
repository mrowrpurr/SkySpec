cmake_minimum_required(VERSION 3.21)

project(SkySpecRunner VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkySpecRunner_Files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE SkySpecRunner_Headers ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SkySpecRunner_Cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(SkySpecRunner ${SkySpecRunner_Files})
target_sources(SkySpecRunner PRIVATE ${SkySpecRunner_Cpp})
target_include_directories(SkySpecRunner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

#if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
#endif()

install(
	TARGETS "${PROJECT_NAME}"
	EXPORT "${PROJECT_NAME}-targets"
)

install(
	EXPORT "${PROJECT_NAME}-targets"
	NAMESPACE "${PROJECT_NAME}::"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
	DIRECTORY
		"include/SkySpec"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
