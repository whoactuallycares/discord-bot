cmake_minimum_required (VERSION 3.20)

add_library(user SHARED "src/main.cpp")
target_link_libraries(user PUBLIC dpp)
set_target_properties(user
	PROPERTIES
	CXX_STANDARD 23)