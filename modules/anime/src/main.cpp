#include "commands.hpp"

extern "C" __declspec(dllexport) const command_header commands[] = {
	{.name = "waifu", .description = "Get an image of a random waifu", .signature = "waifu", .func = command_waifu},
	{.name = "coolcode", .description = "Get an nhentai code", .signature = "coolcode", .func = command_coolcode},
};

extern "C" __declspec(dllexport) const module_header modInfo {
    .name = "anime",
    .description = "Weeb stuff",
    .version = MODULE_MAKE_VERSION(0, 1, 0),
    .nCommands = sizeof(commands) / sizeof(commands[0]),
};