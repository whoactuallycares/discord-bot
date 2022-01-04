#pragma once
#include <dpp/dpp.h>

inline void command_ping(dpp::cluster& _bot, const dpp::message_create_t& _event, std::vector<std::string_view> _args)
{
	_bot.message_create(dpp::message(_event.msg.channel_id, "Pong!"));
};