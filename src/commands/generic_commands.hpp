#pragma once
#include <dpp/dpp.h>

inline void command_ping(dpp::cluster& _bot, const dpp::message_create_t& event, std::vector<std::string> _args)
{
	_bot.message_create(dpp::message(event.msg.channel_id, "Pong!"));
};