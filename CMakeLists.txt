cmake_minimum_required (VERSION 3.13)

# Initialize project
project ("Bot" CXX)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory("${CMAKE_SOURCE_DIR}/lib/DPP")
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/fmt")

add_executable(bot "src/main.cpp")
target_include_directories(bot PRIVATE "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/lib/DPP/include" "${CMAKE_SOURCE_DIR}/lib/mpg123/src" "/usr/include/opus/" "${CMAKE_BINARY_DIR}")

if (UNIX)
	target_link_libraries(bot PRIVATE dpp ogg opus opusfile pthread fmt)
else()
	target_link_libraries(bot PRIVATE dpp fmt)
endif (UNIX)

# Create folder for stroring downloaded music
make_directory("${CMAKE_BINARY_DIR}/songs")

# Configure help file
set(HELP "")
set(COMMANDS "")
file(READ "commands.csv" data)
string(REPLACE "\n" ";" data "${data}")
foreach(COMMAND_DESCRIPTION IN LISTS data)
  string(REPLACE "," ";" COMMAND_DESCRIPTION "${COMMAND_DESCRIPTION}")
  list(POP_FRONT COMMAND_DESCRIPTION COMMAND_NAME)
  string(APPEND HELP
  "\n\t\t\tadd_field(\"${COMMAND_NAME}\",\"${COMMAND_DESCRIPTION}\")."
  )
  string(APPEND COMMANDS
  "\n\telse if (!std::strncmp(_args[0].data(), \"${COMMAND_NAME}\", _args[0].size())) { command_${COMMAND_NAME}(_bot, _event, _args); }"
  )
endforeach()

configure_file("src/select_command.in.hpp" "select_command.hpp")

# Add token to main file
file(READ ".token" TOKEN)
configure_file("src/main.in.cpp" "src/main.cpp")