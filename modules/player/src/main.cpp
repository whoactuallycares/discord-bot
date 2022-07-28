#include "commands.hpp"

extern "C" __declspec(dllexport) const command_header commands[] = {
	{.name = "join", .description = "Join current voice chat", .signature = "join", .func = command_join},
	{.name = "leave", .description = "Leave current vocie chat", .signature = "leave", .func = command_leave},
	{.name = "play", .description = "Play a video from youtube", .signature = "play `string`:name", .func = command_play},
	{.name = "queue", .description = "Show videos in queue", .signature = "queue", .func = command_queue},
};

extern "C" __declspec(dllexport) const module_header modInfo {
    .name = "player",
    .description = "Play videos from youtube in voice chat",
    .version = MODULE_MAKE_VERSION(0, 1, 0),
    .nCommands = sizeof(commands) / sizeof(commands[0]),
};