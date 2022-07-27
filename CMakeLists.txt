cmake_minimum_required (VERSION 3.20)


project(cb CXX)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory("core")

option(CB_BUILD_MODULES "Build modules" ON)
if(${CB_BUILD_MODULES})
	add_subdirectory("modules")
	add_dependencies(bot CB_MODULES_ALL)
endif()