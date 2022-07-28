#pragma once
#include <functional>
#include <dpp/dpp.h>
#include <string_view>
#define MODULE_MAKE_VERSION(major, minor, patch) (((major & 0x3ff) << 20) | ((minor & 0x3ff) << 10) | (patch & 0x3ff))
#define MODULE_VERSION_MAJOR(version) ((version >> 20) & 0x3ff)
#define MODULE_VERSION_MINOR(version) ((version >> 10) & 0x3ff)
#define MODULE_VERSION_PATCH(version) (version & 0x3ff)

struct module_header
{
	const char* name;
	const char* description;
	uint32_t version;
	uint32_t nCommands;
	////
	void* handle;
	uint32_t iFirstCommand;
};

using command_function = std::function<void(dpp::cluster&, const dpp::message_create_t&, const std::vector<std::string_view>&)>;
struct command_header
{
	const char* name;
	const char* description;
	const char* signature;
	command_function func;
};