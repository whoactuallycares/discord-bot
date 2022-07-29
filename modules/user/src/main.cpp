#include "commands.hpp"

extern "C" __declspec(dllexport) const command_header commands[] = {
	{.name = "message", .description = "Send a message to a user", .signature = "message `user`:user", .func = command_message},
};

extern "C" __declspec(dllexport) const module_header modInfo {
    .name = "user",
    .description = "Stuff to do with the users and their accounts",
    .version = MODULE_MAKE_VERSION(0, 1, 0),
    .nCommands = sizeof(commands) / sizeof(commands[0]),
};
