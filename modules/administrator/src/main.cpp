#include "commands.hpp"

extern "C" __declspec(dllexport) const command_header commands[] = {
	{.name = "ban--", .description = "Banish a user from the server", .signature = "ban `user`:user", .func = command_ban},
	{.name = "unban--", .description = "Revoke banishment of a user", .signature = "unban `user`:user", .func = command_unban},
	{.name = "mute", .description = "Mute a user", .signature = "mute `user`:user", .func = command_mute},
	{.name = "unmute", .description = "Unmute a user", .signature = "unmute `user`:user", .func = command_unmute},
	{.name = "channel_create", .description = "Create a new text channel", .signature = "channel_create", .func = command_channel_create},
};

extern "C" __declspec(dllexport) const module_header modInfo {
    .name = "administrator",
    .description = "Control and administrate the server",
    .version = MODULE_MAKE_VERSION(0, 1, 0),
    .nCommands = sizeof(commands) / sizeof(commands[0]),
};
