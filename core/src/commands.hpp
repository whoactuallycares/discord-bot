#pragma once
#include <iostream>
#include <dpp/dpp.h>
#include <string>
#include <string_view>
#include <tuple>

inline void command_ping(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	_bot.message_create(dpp::message(_event.msg.channel_id, "Pong!"));
};
uint32_t getNModules();
std::vector<std::pair<std::string, std::function<void(dpp::cluster&, const dpp::message_create_t&, const std::vector<std::string_view>&)>>>& getcommands();
void* loadmod(const char*);
inline void command_info(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	dpp::embed embed = dpp::embed().
		set_color(0xfafafa).
		add_field("Version", "v0.1.4", true).
		add_field("Modules", std::to_string(getNModules()), true).
		add_field("Commands", std::to_string(getcommands().size()), true).
		set_footer(dpp::embed_footer().set_text(":3")).
		set_timestamp(time(0));
	_bot.message_create(dpp::message(_event.msg.channel_id, embed));
};

inline void command_modules(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	if (_args.size() < 2)
		return;
	if (_args[1] == "list")
	{
		std::string list;
		_bot.message_create(dpp::message(_event.msg.channel_id, list));
	}
	else if (_args.size() >= 3)
	{
		if (_args[1] == "load") // TODO : Accept multiple modules
		{
			std::string modname = { _args[2].data(), _args[2].size() };
			modname += ".dll";
			if(loadmod(modname.c_str()))
				_bot.message_create(dpp::message(_event.msg.channel_id, "success"));
			else
				_bot.message_create(dpp::message(_event.msg.channel_id, "failed"));
		}
		else if (_args[1] == "unload")
		{

		};
	};
};
