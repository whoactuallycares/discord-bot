cmake_minimum_required (VERSION 3.20)

project("Bot" CXX)

#add_compile_definitions(FD_SETSIZE=16384)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/DPP")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/fmt")

add_executable(bot "src/main.cpp" "src/module_manager.cpp" "src/group_manager.cpp")
set_target_properties(bot
	PROPERTIES
	CXX_STANDARD 23)
target_include_directories(bot PUBLIC
"${CMAKE_SOURCE_DIR}/common"
"${CMAKE_CURRENT_BINARY_DIR}/src")

target_link_libraries(bot PUBLIC dpp fmt)
if (UNIX)
	target_link_libraries(bot PUBLIC pthread)
endif()

# Add token to main file
file(READ "${CMAKE_SOURCE_DIR}/.token" TOKEN)
configure_file("src/token.in.hpp" "src/token.hpp")