#include <dpp/dpp.h>
#include <string>
#include <string_view>
#include <headers.hpp>

inline void command_disconnect(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	_bot.message_create(dpp::message(_event.msg.channel_id, "Player1!"));
};

inline void command_ban(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	if (_event.msg.mentions.size() != 1)
	{
		_bot.message_create(dpp::message(_event.msg.channel_id, "You need to mention 1 person"));
		return;
	};

	_bot.guild_ban_add(_event.msg.guild_id, _event.msg.mentions[0].first.id, 0);
	_bot.message_create(dpp::message(_event.msg.channel_id, "Banned someone?"));
};

inline void command_unban(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	if (_event.msg.mentions.size() != 1)
	{
		_bot.message_create(dpp::message(_event.msg.channel_id, "You need to mention 1 person"));
		return;
	};
	_bot.guild_ban_delete(_event.msg.guild_id, _event.msg.mentions[0].first.id);
	_bot.message_create(dpp::message(_event.msg.channel_id, "Player1!"));
};

inline void command_mute(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	if (_event.msg.mentions.size() != 1)
	{
		_bot.message_create(dpp::message(_event.msg.channel_id, "You need to mention 1 person"));
		return;
	};
	dpp::guild_member gm;
	gm.guild_id = _event.msg.guild_id;
	gm.user_id = _event.msg.mentions[0].first.id;
	gm.set_mute(true);
	_bot.guild_edit_member(gm);

};

inline void command_unmute(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	if (_event.msg.mentions.size() != 1)
	{
		_bot.message_create(dpp::message(_event.msg.channel_id, "You need to mention 1 person"));
		return;
	};
	dpp::guild_member gm;
	gm.guild_id = _event.msg.guild_id;
	gm.user_id = _event.msg.mentions[0].first.id;
	gm.set_mute(false);
	_bot.guild_edit_member(gm);
};

inline void command_channel_create(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	dpp::channel channel;
	channel.set_guild_id(_event.msg.guild_id);
	channel.set_name("channel");
	_bot.channel_create(channel);
};
