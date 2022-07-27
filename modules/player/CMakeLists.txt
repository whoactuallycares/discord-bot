cmake_minimum_required (VERSION 3.20)

set(OP_DISABLE_DOCS ON CACHE BOOL "" FORCE)
set(INSTALL_DOCS OFF CACHE BOOL "" FORCE)
if(NOT UNIX)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON CACHE BOOL "" FORCE) # Necessary for opusfile.lib to generate
endif()

add_subdirectory("lib/ogg")
add_subdirectory("lib/opus")
add_subdirectory("lib/opusfile")

add_library(player SHARED "src/main.cpp" "src/player.cpp")
target_link_libraries(player PUBLIC dpp fmt ogg opus opusfile)
target_include_directories(player PUBLIC
"${CMAKE_CURRENT_SOURCE_DIR}/lib/ogg/includ"
"${CMAKE_CURRENT_SOURCE_DIR}/lib/opus/includ"
"${CMAKE_CURRENT_SOURCE_DIR}/lib/opusfile/opusfile"
)
set_target_properties(player
	PROPERTIES
	CXX_STANDARD 23)
