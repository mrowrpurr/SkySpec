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

install(TARGETS SkySpecRunner
	EXPORT SkySpecRunner-config
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)

install(EXPORT SkySpecRunner-config
	FILE SkySpecRunner-config.cmake
	NAMESPACE SkySpecRunner::
	DESTINATION lib/cmake/SkySpecRunner
)

install(FILES ${SkySpec_Runner_Headers} DESTINATION include)

install(
	DIRECTORY
		"include/SkySpec"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
