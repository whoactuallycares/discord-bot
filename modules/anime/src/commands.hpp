#include <dpp/dpp.h>
#include <string>
#include <string_view>
#include <headers.hpp>
#include <asserts.hpp>

inline void command_waifu(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	_bot.message_create(dpp::message(_event.msg.channel_id, "https://www.thiswaifudoesnotexist.net/example-" + std::to_string(std::rand() % 100000) + ".jpg"));
};

inline void command_coolcode(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	const std::string codes[] = {
#include "codes.hpp"
	};
	_bot.message_create(dpp::message(_event.msg.channel_id, "`" + codes[std::rand() % (sizeof(codes) / sizeof(codes[0]))] + "` is very cool!"));
};