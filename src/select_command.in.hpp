#pragma once
#include <string>
#include <dpp/dpp.h>
#include "commands/commands.hpp"

inline void select_command(dpp::cluster& _bot, const dpp::message_create_t& _event, std::vector<std::string> _args)
{
	if (!strcmp(_args[0].c_str(), "help"))
	{
		dpp::embed embed = dpp::embed().
			set_color(0xfafafa).
			set_title("Help").@HELP@
			set_footer(dpp::embed_footer().set_text(":3")).
			set_timestamp(time(0));
		_bot.message_create(dpp::message(_event.msg.channel_id, embed));
	}@COMMANDS@
	else
	{
		_bot.message_create(dpp::message(_event.msg.channel_id, "Unknown command \"" + _args[0] + "\". Type `!help` for more information."));
	};
};