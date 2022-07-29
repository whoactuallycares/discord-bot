cmake_minimum_required (VERSION 3.20)

add_library(anime SHARED "src/main.cpp")
target_link_libraries(anime PUBLIC dpp)
set_target_properties(anime
	PROPERTIES
	CXX_STANDARD 23)