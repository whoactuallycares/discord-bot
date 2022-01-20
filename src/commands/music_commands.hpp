#pragma once
#include <dpp/dpp.h>
#include <player/player.hpp>
#include <fmt/core.h>
#include <utils/random.hpp>
#include <utils/clock.hpp>
#ifdef __linux__
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ogg/ogg.h>
#include <opus/opusfile.h>
#endif // __linux__
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

inline void command_join(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args);
inline void command_leave(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args);

inline void command_play(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	if (_event.msg.filename.size() > 0)
	{
        std::cout << "Oooh a file: " << _event.msg.filename << "\n";
		_bot.message_create(dpp::message(_event.msg.channel_id, "Tf you sending a file for m8?"));
	}
    else if (_args.size() > 1)
    {
		player& p = players[_event.msg.guild_id];
        if (_event.msg.content.find("http://") != std::string::npos || _event.msg.content.find("https://") != std::string::npos)
        {
            if (_event.msg.content.find("spotify.com/") != std::string::npos)
            {
                _bot.message_create(dpp::message(_event.msg.channel_id, "spotDL" + std::string(_args[1])));
            }
            else
            {
				p.song_add(yt_dlp_url(std::string(_args[1])));
            };
        }
        else
        {
			p.song_add(yt_dlp_search(std::string(_args[1].data(), _event.msg.content.size() - (_args[1].data() - _args[0].data() + 1))));
        };

		dpp::embed embed = dpp::embed().
			set_color(0xfafafa).
			set_title(p[p.nSongs() - 1].title).
			set_url(p[p.nSongs() - 1].url).
			set_description(fmt::format("\
				Length : **{}**\
				Position in queue : **{}**\
			", p[p.nSongs() - 1].duration, p.nSongs()
			)).
			set_image(p[p.nSongs() - 1].thumbnail).
			set_footer(dpp::embed_footer().set_text(":3")).
			set_timestamp(time(0));
		_bot.message_create(dpp::message(_event.msg.channel_id, embed));

		p.resume();
        players.serialize("test.bin");
		auto v = _event.from->get_voice(_event.msg.guild_id);
		if(!v || !v->voiceclient || !v->voiceclient->is_ready())
		{
			command_join(_bot, _event, _args);
			return;
		};
	}
	else
	{
		_bot.message_create(dpp::message(_event.msg.channel_id, "You need to tell me what to play dipshit."));
	};
};

inline void command_join(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	player& p = players[_event.msg.guild_id];
	dpp::voiceconn* v = _event.from->get_voice(_event.msg.guild_id);
	auto users = dpp::find_guild(_event.msg.guild_id)->voice_members;
	if (v && users.find(_event.msg.author.id) != users.end() && v->channel_id != users.find(_event.msg.author.id)->second.channel_id)
	{
		command_leave(_bot, _event, _args);
	};
    if (!dpp::find_guild(_event.msg.guild_id)->connect_member_voice(_event.msg.author.id))
    {
        _bot.message_create(dpp::message(_event.msg.channel_id, "You need to be in a voicechannel."));
        return;
    };
	_bot.message_create(dpp::message(_event.msg.channel_id, "Connected"));
	//_bot.message_create(dpp::message(_event.msg.channel_id, fmt::format("gi: {}\npg: {}", _event.msg.guild_id, a[_event.msg.guild_id]++)));
};

inline void command_leave(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	player& p = players[_event.msg.guild_id];
	_event.from->disconnect_voice(_event.msg.guild_id);
	//_bot.message_create(dpp::message(_event.msg.channel_id, fmt::format("gi: {}\npg: {}", _event.msg.guild_id, a[_event.msg.guild_id]++)));
	p.pause();
};

inline void command_queue(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	player& p = players[_event.msg.guild_id];
    dpp::embed embed = dpp::embed().
        set_color(0xfafafa).
        set_title("Song queue").
        set_description(fmt::format("\
            Total length : **{} ({} songs)**\
            Loop : **no**\
            Effects : **none**\
		", time_to_clock(p.length()), p.nSongs()
        )).
        set_footer(dpp::embed_footer().set_text(":3")).
        set_timestamp(time(0));
	uint32_t i = 0;
    for (auto song : p)
    {
        embed.add_field(
            fmt::format("`{}` - {}", ++i, song.title),
            fmt::format("`{}`", song.duration)
        );
    };
    _bot.message_create(dpp::message(_event.msg.channel_id, embed));
};

inline void command_skip(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	player& p = players[_event.msg.guild_id];
    p.song_remove(0);
	p.skip();
};

inline void command_jump(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	player& p = players[_event.msg.guild_id];
    if (_args.size() > 1)
    {
        uint32_t iSong = std::stoi(std::string(_args[1]));
        if (iSong != 0)
        {
            for (uint32_t i = 1; i < iSong; i++)
            {
                p.songs_swap(i - 1, i);
            };
        };
    };
};

inline void command_move(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	player& p = players[_event.msg.guild_id];
    if (_args.size() > 2)
    {
        uint32_t iSong1 = std::stoi(std::string(_args[1]));
        uint32_t iSong2 = std::stoi(std::string(_args[2]));
        if (iSong1 != iSong2)
        {
            for (uint32_t i = iSong1; i < iSong2; i++)
            {
                p.songs_swap(i - 1, i);
            };
        };
    };
};

inline void command_nowplaying(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
};

inline void command_pause(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	player& p = players[_event.msg.guild_id];
	p.pause();
};

inline void command_resume(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	player& p = players[_event.msg.guild_id];
	p.resume();
};

inline void command_effects(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
};

inline void command_shuffle(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	player& p = players[_event.msg.guild_id];
	std::shuffle(p.begin(), p.end(), bot::rand::rng);
};