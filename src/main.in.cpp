#include <iostream>
#include <vector>
#include <string>
#include <dpp/dpp.h>
#include <dpp/fmt/format.h>
#include "player/player.hpp"
//#include "per_guild.hpp"
#include <select_command.hpp>
#include <string_view>

std::vector<std::string_view> command_parse(std::string_view _msg)
{
    std::vector<std::string_view> args;
    uint32_t end = 1, beg;
    while (_msg[end])
    {
        beg = end;
		for (; _msg.size() != end && (_msg[end] != ' '); end++);
		args.push_back(_msg.substr(beg, end - beg));
		while (_msg[end] && _msg[++end] == ' ');
    };
    return args;
};

// Find a string in an array of strings that begins with this string
uint32_t find(std::string _str, std::vector<std::string> _strs)
{
    uint32_t i = 0;
    uint32_t j = 0;
    start:
    if (_str[j] == _strs[i][j])
    {
        if (_str[j] == _strs[i + 1][j])
        {
            // ambiguous
            if (j + 1 < _str.size())
                j++;
            else
                return ~0; // either ambiguous or wrong
            goto start;
        }
        else
        {
            //it's this one
            return i; // either right or wrong
        };
    }
    else
    {
		if (i + 1 < _strs.size())
			i++;
		else
			return ~0; // didnt find
        goto start;
    };

    while (true)
    {
		if (_str[j] == _strs[i][j])
		{
			if (_str[j] == _strs[i + 1][j])
			{
				if (j + 1 < _str.size())
					j++;
				else
					return ~0; // Ambiguous
			}
			else
			{
                while ((_str[j] && _strs[i][j]) && (_str[j++] == _strs[i][j]));
				return !_str[j] ? i : ~0; // This one or pattern doesn't match
			};
		}
		else
		{
			if (i + 1 < _strs.size())
				i++;
			else
				return ~0; // Didn't find
		};
    };
};

int main()
{
    // Create the bot
    dpp::cluster bot("@TOKEN@");

     //The interaction create event is fired when someone issues your commands 
    bot.on_interaction_create([&bot](const dpp::interaction_create_t& event) {
        if (event.command.type == dpp::it_application_command) {
            dpp::command_interaction cmd_data = std::get<dpp::command_interaction>(event.command.data);
            // Check which command they ran
            if (cmd_data.name == "blep") {
                // Fetch a parameter value from the command parameters
                std::string animal = std::get<std::string>(event.get_parameter("animal"));
                // Reply to the command. There is an overloaded version of this
                // call that accepts a dpp::message so you can send embeds.

                event.reply(dpp::ir_channel_message_with_source, fmt::format("Blep! You chose {}", animal));
            }
        }
	});

    bot.on_ready([&bot](const auto& event) {
        std::cout << "Logged in as " << bot.me.username << "!\n";


        dpp::slashcommand newcommand;
        // Create a new global command on ready event
        newcommand.set_name("blep")
            .set_description("Send a random adorable animal photo")
            .set_application_id(bot.me.id)
            .add_option(
                dpp::command_option(dpp::co_string, "animal", "The type of animal", true).
                add_choice(dpp::command_option_choice("Dog", std::string("animal_dog"))).
                add_choice(dpp::command_option_choice("Cat", std::string("animal_cat"))).
                add_choice(dpp::command_option_choice("Penguin", std::string("animal_penguin")
                )
                )
            );

        // Register the command
        bot.global_command_create(newcommand);
	});

    bot.on_message_create([&bot](const auto& event) {
        if (event.msg.content[0] != '!')
        {
            return;
        };
        select_command(bot, event, command_parse(event.msg.content));
	});

    bot.start(false);
    return 0;
};



#ifdef HGH
#include <dpp/dpp.h>
#include <iomanip>
#include <sstream>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ogg/ogg.h>
#include <opus/opusfile.h>

int main(int argc, char const* argv[])
{
    dpp::voiceconn v;
    /* Load an ogg opus file into memory.
     * The bot expects opus packets to be 2 channel stereo, 48000Hz.
     *
     * You may use ffmpeg to encode songs to ogg opus:
     * ffmpeg -i /path/to/song -c:a libopus -ar 48000 -ac 2 -vn -b:a 96K /path/to/opus.ogg
     */
    dpp::cluster bot("@TOKEN@");

    bot.on_ready([&bot](const auto& event) {
        std::cout << "Logged in as " << bot.me.username << "!\n";


        dpp::slashcommand newcommand;
        // Create a new global command on ready event
        newcommand.set_name("blep")
            .set_description("Send a random adorable animal photo")
            .set_application_id(bot.me.id)
            .add_option(
                dpp::command_option(dpp::co_string, "animal", "The type of animal", true).
                add_choice(dpp::command_option_choice("Dog", std::string("animal_dog"))).
                add_choice(dpp::command_option_choice("Cat", std::string("animal_cat"))).
                add_choice(dpp::command_option_choice("Penguin", std::string("animal_penguin")
                )
                )
            );

        // Register the command
        bot.global_command_create(newcommand);
	});

    /* Use the on_message_create event to look for commands */
    bot.on_message_create([&bot](const dpp::message_create_t& event) {

        /* Tell the bot to join the discord voice channel the user is on. Syntax: .join */
        if (event.msg.content == "!join") {
            dpp::guild* g = dpp::find_guild(event.msg.guild_id);
            bot.message_create(dpp::message(event.msg.channel_id, g->name));
            if (!g->connect_member_voice(event.msg.author.id, false, true)) {
                bot.message_create(dpp::message(event.msg.channel_id, "You don't seem to be on a voice channel! :("));
            }
            else
            {
                bot.message_create(dpp::message(event.msg.channel_id, "Apparently im here"));
            };
        }

        /* Tell the bot to play the sound file */
        if (event.msg.content == "!play") {
            system("/home/tommy/.local/bin/yt-dlp --extract-audio --audio-format opus -o \"test.opus\" https://www.youtube.com/watch?v=DWCPCZqZnTU");
            dpp::voiceconn* v = event.from->get_voice(event.msg.guild_id);
            if (v && v->voiceclient && v->voiceclient->is_ready()) {
                ogg_sync_state oy;
                ogg_stream_state os;
                ogg_page og;
                ogg_packet op;
                OpusHead header;
                char* buffer;

                FILE* fd;

                fd = fopen("test.opus", "rb");

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

                        v->voiceclient->send_audio_opus(op.packet, op.bytes, samples / 48);
                    }
                }

                /* Cleanup */
                ogg_stream_clear(&os);
                ogg_sync_clear(&oy);
            }
        }
	});

    bot.on_log([&bot](const dpp::log_t& event) {
        std::cout << event.message << "\n";
	});

    /* Start bot */
    bot.start(false);
    return 0;
}
#endif