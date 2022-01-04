#pragma once
#include <dpp/dpp.h>
#include <player/player.hpp>
#include <fmt/core.h>
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

player q;
inline void command_join(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args);
inline void command_leave(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args);

inline void command_play(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	command_join(_bot, _event, _args);

	if (_event.msg.filename.size() > 0)
	{
		_bot.message_create(dpp::message(_event.msg.channel_id, "a File"));
	}
    else if (_args.size() > 1)
    {
        std::cout << _event.msg.filename << "\n";
        if (_event.msg.content.find("http://") != std::string::npos || _event.msg.content.find("https://") != std::string::npos)
        {
            if (_event.msg.content.find("spotify.com/") != std::string::npos)
            {
                _bot.message_create(dpp::message(_event.msg.channel_id, "spotDL" + std::string(_args[1])));
            }
            else
            {
				q.song_add(yt_dlp_url(std::string(_args[1])));
            };
        }
        else
        {
			q.song_add(yt_dlp_search(std::string(_args[1].data(), _event.msg.content.size() - (_args[1].data() - _args[0].data() + 1))));
        };
		dpp::embed embed = dpp::embed().
			set_color(0xfafafa).
			set_title(q[q.nSongs() - 1].title).
            set_url(q[q.nSongs() - 1].url).
			set_description(fmt::format("\
				Length : **{}**\
				Position in queue : **{}**\
			", q[q.nSongs() - 1].duration, q.nSongs()
			)).
            set_image(q[q.nSongs() - 1].thumbnail).
			set_footer(dpp::embed_footer().set_text(":3")).
			set_timestamp(time(0));
		_bot.message_create(dpp::message(_event.msg.channel_id, embed));
		q.playing = true;
		// play
		if (q.voiceConnection && q.voiceConnection->voiceclient && q.voiceConnection->voiceclient->is_ready())
		{
#ifdef __linux__ 
			ogg_sync_state oy;
			ogg_stream_state os;
			ogg_page og;
			ogg_packet op;
			OpusHead header;
			char* buffer;

			FILE* fd = nullptr;
			for (uint32_t i = 0; i < 100; i++)
			{
				std::this_thread::sleep_for(50ms);
				fd = fopen(std::string("songs/" + q[0].id + ".opus").c_str(), "rb");
			};
			if (!fd)
			{
				_bot.message_create(dpp::message(_event.msg.channel_id, "Failed to open audio file"));
				return;
			};
			_bot.message_create(dpp::message(_event.msg.channel_id, "Playing " + std::string(_args[1])));

			fseek(fd, 0L, SEEK_END);
			size_t sz = ftell(fd);
			rewind(fd);

			ogg_sync_init(&oy);

			int eos = 0;
			int i;

			buffer = ogg_sync_buffer(&oy, sz);
			fread(buffer, 1, sz, fd);

			ogg_sync_wrote(&oy, sz);

			if (ogg_sync_pageout(&oy, &og) != 1)
			{
				fprintf(stderr, "Does not appear to be ogg stream.\n");
				exit(1);
			}

			ogg_stream_init(&os, ogg_page_serialno(&og));

			if (ogg_stream_pagein(&os, &og) < 0) {
				fprintf(stderr, "Error reading initial page of ogg stream.\n");
				exit(1);
			}

			if (ogg_stream_packetout(&os, &op) != 1)
			{
				fprintf(stderr, "Error reading header packet of ogg stream.\n");
				exit(1);
			}

			/* We must ensure that the ogg stream actually contains opus data */
			if (!(op.bytes > 8 && !memcmp("OpusHead", op.packet, 8)))
			{
				fprintf(stderr, "Not an ogg opus stream.\n");
				exit(1);
			}

			/* Parse the header to get stream info */
			int err = opus_head_parse(&header, op.packet, op.bytes);
			if (err)
			{
				fprintf(stderr, "Not a ogg opus stream\n");
				exit(1);
			}
			/* Now we ensure the encoding is correct for Discord */
			if (header.channel_count != 2 && header.input_sample_rate != 48000)
			{
				fprintf(stderr, "Wrong encoding for Discord, must be 48000Hz sample rate with 2 channels.\n");
				exit(1);
			}

			/* Now loop though all the pages and send the packets to the vc */
			while (ogg_sync_pageout(&oy, &og) == 1) {
				ogg_stream_init(&os, ogg_page_serialno(&og));

				if (ogg_stream_pagein(&os, &og) < 0) {
					fprintf(stderr, "Error reading page of Ogg bitstream data.\n");
					exit(1);
				}

				while (ogg_stream_packetout(&os, &op) != 0)
				{
					/* Read remaining headers */
					if (op.bytes > 8 && !memcmp("OpusHead", op.packet, 8))
					{
						int err = opus_head_parse(&header, op.packet, op.bytes);
						if (err)
						{
							fprintf(stderr, "Not a ogg opus stream\n");
							exit(1);
						}
						if (header.channel_count != 2 && header.input_sample_rate != 48000)
						{
							fprintf(stderr, "Wrong encoding for Discord, must be 48000Hz sample rate with 2 channels.\n");
							exit(1);
						}
						continue;
					}
					/* Skip the opus tags */
					if (op.bytes > 8 && !memcmp("OpusTags", op.packet, 8))
						continue;

					/* Send the audio */
					int samples = opus_packet_get_samples_per_frame(op.packet, 48000);

					q.voiceConnection->voiceclient->send_audio_opus(op.packet, op.bytes, samples / 48);
				}
			}

			/* Cleanup */
			ogg_stream_clear(&os);
			ogg_sync_clear(&oy);
#endif // __linux__
		};
    };
};

inline void command_join(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	auto current_vc = _event.from->get_voice(_event.msg.guild_id);
	auto users = dpp::find_guild(_event.msg.guild_id)->voice_members;
	if (_event.from->get_voice(_event.msg.guild_id) && users.find(_event.msg.author.id) != users.end() && current_vc->channel_id == users.find(_event.msg.author.id)->second.channel_id)
	{
		command_leave(_bot, _event, _args);
	};
    if (!dpp::find_guild(_event.msg.guild_id)->connect_member_voice(_event.msg.author.id, false, true))
    {
        _bot.message_create(dpp::message(_event.msg.channel_id, "You need to be in a voicechannel."));
        return;
    };
};

inline void command_leave(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	_event.from->disconnect_voice(_event.msg.guild_id);
};

inline void command_queue(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
    dpp::embed embed = dpp::embed().
        set_color(0xfafafa).
        set_title("Song queue").
        set_description(fmt::format("\
            Total length : **{} ({} songs)**\
            Loop : **no**\
            Effects : **none**\
		", time_to_clock(q.length()), q.nSongs()
        )).
        set_footer(dpp::embed_footer().set_text(":3")).
        set_timestamp(time(0));
	uint32_t i = 0;
    for (auto song : q)
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
    q.song_remove(0);
};

inline void command_jump(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
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

inline void command_move(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
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

inline void command_nowplaying(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
};

inline void command_pause(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	if (q.voiceConnection && q.voiceConnection->voiceclient && q.voiceConnection->voiceclient->is_ready())
	{
		q.voiceConnection->voiceclient->pause_audio(true);
	};
};

inline void command_resume(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	if (q.voiceConnection && q.voiceConnection->voiceclient && q.voiceConnection->voiceclient->is_ready())
	{
		q.voiceConnection->voiceclient->pause_audio(false);
	};
};

inline void command_effects(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
};

inline void command_shuffle(dpp::cluster& _bot, const dpp::message_create_t& _event, const std::vector<std::string_view>& _args)
{
	std::shuffle(q.begin(), q.end(), bot::rand::rng);
};