#include <iostream>
#include <dpp/dpp.h>
#include <functional>
#include <string>
#include <string_view>
#include <headers.hpp>

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

extern "C" __declspec(dllexport) const command_header commands[] = {
	{.name = "ban--", .description = "Banish a user from the server", .signature = "ban `user`:user", .func = command_ban},
	{.name = "unban--", .description = "Revoke banishment of a user", .signature = "unban `user`:user", .func = command_unban},
	{.name = "mute", .description = "Mute a user", .signature = "mute `user`:user", .func = command_mute},
	{.name = "unmute", .description = "Unmute a user", .signature = "unmute `user`:user", .func = command_unmute},
	{.name = "channel_create", .description = "Create a new text channel", .signature = "channel_create", .func = command_channel_create},
};

extern "C" __declspec(dllexport) const module_header modInfo {
    .name = "administrator",
    .description = "Control and administrate the server",
    .version = MODULE_MAKE_VERSION(0, 1, 0),
    .nCommands = sizeof(commands) / sizeof(commands[0]),
};
