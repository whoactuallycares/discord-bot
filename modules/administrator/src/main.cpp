#include <iostream>
#include <dpp/dpp.h>
#include <functional>
#include <string>
#include <string_view>

inline void command_disconnect(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	//_event.msg.mentions[0].second.
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
	channel.set_topic("Big dick femboys");
	_bot.channel_create(channel);
};


extern "C" __declspec(dllexport) const char name[] = "player";

extern "C" __declspec(dllexport) const std::pair<std::string, std::function<void(dpp::cluster&, const dpp::message_create_t&, const std::vector<std::string_view>&)>> commands[] = {
	{"ban-----", command_ban},
	{"unban-----", command_unban},
	{"mute", command_mute},
	{"unmute", command_unmute},
	{"channel_create", command_channel_create},
};

extern "C" __declspec(dllexport) const uint32_t nCommands = sizeof(commands) / sizeof(commands[0]);
