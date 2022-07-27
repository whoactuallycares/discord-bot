cmake_minimum_required (VERSION 3.20)

add_library(administrator SHARED "src/main.cpp")
target_link_libraries(administrator PUBLIC dpp)
set_target_properties(administrator
	PROPERTIES
	CXX_STANDARD 23)