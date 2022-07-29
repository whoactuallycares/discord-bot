#include <dpp/dpp.h>
#include <string>
#include <string_view>
#include <headers.hpp>
#include <asserts.hpp>

inline void command_message(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	if (!has_mentions(_event.msg))
		return;

	const auto userId = _event.msg.mentions[0].first.id;
	const auto channelId = _bot.get_dm_channel(userId);
	if(!_bot.get_dm_channel(userId))
		_bot.create_dm_channel(userId, [&_bot](const dpp::confirmation_callback_t& _cbArgs) {
		if (_cbArgs.is_error())
			std::cout << "error\n";
		else
			std::cout << "Good\n";
		_bot.message_create(dpp::message(std::get<dpp::channel>(_cbArgs.value).id, "hello"));
	});

};
