#pragma once
#include <dpp/dpp.h>
#include <player/player.hpp>

player q;

inline void command_play(dpp::cluster& _bot, const dpp::message_create_t& event, std::vector<std::string> _args)
{
    // join the vc
    if (!dpp::find_guild(event.msg.guild_id)->connect_member_voice(event.msg.author.id, false, true))
    {
        _bot.message_create(dpp::message(event.msg.channel_id, "You need to be in a voicechannel."));
    };

    if (_args.size() > 1)
        q.song_add({ _args[1], 100 });
    q.playing_ = true;
    // play
    dpp::voiceconn* v = event.from->get_voice(event.msg.guild_id);
    if (v && v->voiceclient && v->voiceclient->is_ready())
    {
        _bot.message_create(dpp::message(event.msg.channel_id, "Playing " + _args[0]));
    };
};

inline void command_queue(dpp::cluster& _bot, const dpp::message_create_t& event, std::vector<std::string> _args)
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

inline void command_skip(dpp::cluster& _bot, const dpp::message_create_t& event, std::vector<std::string> _args)
{
    q.song_remove(0);
};

inline void command_jump(dpp::cluster& _bot, const dpp::message_create_t& event, std::vector<std::string> _args)
{
    if (_args.size() > 1)
    {
        if (std::stoi(_args[1]) != 0)
        {
            for (uint32_t i = 1; i < std::stoi(_args[1]); i++)
            {
                q.songs_swap(i - 1, i);
            };
        };
    };
};

inline void command_move(dpp::cluster& _bot, const dpp::message_create_t& event, std::vector<std::string> _args)
{
    if (_args.size() > 2)
    {
        if (std::stoi(_args[1]) != std::stoi(_args[2]))
        {
            for (uint32_t i = std::stoi(_args[1]); i < std::stoi(_args[2]); i++)
            {
                q.songs_swap(i - 1, i);
            };
        };
    };
};