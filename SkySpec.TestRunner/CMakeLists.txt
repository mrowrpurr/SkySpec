cmake_minimum_required(VERSION 3.21)

project(SkySpec_Runner VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SkySpec_Runner_Files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE SkySpec_Runner_Headers ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SkySpec_Runner_Cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(SkySpec_Runner ${SkySpec_Runner_Files})
add_library(SkySpec::Runner ALIAS SkySpec_Runner)
target_sources(SkySpec_Runner PRIVATE ${SkySpec_Runner_Cpp})
target_include_directories(SkySpec_Runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    install(TARGETS SkySpec_Runner
        EXPORT skyspec-runner-config
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )

    install(EXPORT skyspec-runner-config
        FILE skyspec-runner-config.cmake
        NAMESPACE SkySpec_Runner::
        DESTINATION lib/cmake/skyspec-runner
    )

    install(FILES ${SkySpec_Runner_Headers} DESTINATION include)
endif()

