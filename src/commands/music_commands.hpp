#pragma once
#include <dpp/dpp.h>
#include <player/player.hpp>]

player q;

inline void command_play(dpp::cluster& _bot, const dpp::message_create_t& event, std::vector<std::string_view> _args)
{
    // play file
    // play link
    // play name

    // join the vc
    if (!dpp::find_guild(event.msg.guild_id)->connect_member_voice(event.msg.author.id, false, true))
    {
        _bot.message_create(dpp::message(event.msg.channel_id, "You need to be in a voicechannel."));
    };

    if (_args.size() > 1)
    {
        q.song_add({ _args[1], 100 });
    };

    q.playing_ = true;
    // play
    dpp::voiceconn* v = event.from->get_voice(event.msg.guild_id);
    if (v && v->voiceclient && v->voiceclient->is_ready())
    {
        _bot.message_create(dpp::message(event.msg.channel_id, "Playing " + std::string(_args[0])));
    };
};

inline void command_queue(dpp::cluster& _bot, const dpp::message_create_t& event, std::vector<std::string_view> _args)
{
    dpp::embed embed = dpp::embed().
        set_color(0xfafafa).
        set_title("Queue (0 songs) queue length in time, no effects").
        set_footer(dpp::embed_footer().set_text(":3")).
        set_timestamp(time(0));
    for (auto song : q)
    {
        embed.add_field(
            song.name_,
            "This **song** is " + std::to_string(song.length_) + " seconds"
        );
    };
    _bot.message_create(dpp::message(event.msg.channel_id, embed));
};

inline void command_skip(dpp::cluster& _bot, const dpp::message_create_t& event, std::vector<std::string_view> _args)
{
    q.song_remove(0);
};

inline void command_jump(dpp::cluster& _bot, const dpp::message_create_t& event, std::vector<std::string_view> _args)
{
    if (_args.size() > 1)
    {
        uint32_t iSong = std::stoi(std::string(_args[1]));
        if (iSong != 0)
        {
            for (uint32_t i = 1; i < iSong; i++)
            {
                q.songs_swap(i - 1, i);
            };
        };
    };
};

inline void command_move(dpp::cluster& _bot, const dpp::message_create_t& event, std::vector<std::string_view> _args)
{
    if (_args.size() > 2)
    {
        uint32_t iSong1 = std::stoi(std::string(_args[1]));
        uint32_t iSong2 = std::stoi(std::string(_args[2]));
        if (iSong1 != iSong2)
        {
            for (uint32_t i = iSong1; i < iSong2; i++)
            {
                q.songs_swap(i - 1, i);
            };
        };
    };
};