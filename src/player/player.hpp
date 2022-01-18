#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <utils/random.hpp>
#include <utils/clock.hpp>
#include "yt_search.hpp"
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ogg/ogg.h>
#include <opus/opusfile.h>
#include <thread>
#include <chrono>
#include <per_guild.hpp>
using namespace std::chrono_literals;

class player
{
public:
	player() {};
	~player() {};
	const song& operator[](uint32_t _i)
	{
		return queue_[_i];
	};

	void song_add(const song& _song) { queue_.push_back(_song); };
	void song_remove(uint32_t _i) { queue_.erase(queue_.begin() + _i); };
	void songs_swap(uint32_t _i1, uint32_t _i2) { std::swap(queue_[_i1], queue_[_i2]); };
	uint32_t nSongs() { return queue_.size(); };
	uint32_t length() { uint32_t t = 0; for (auto s : queue_) t += clock_to_time(s.duration); return t; };
	void volume(uint32_t _volume) { volume_ = _volume; };
	void pause() { playing_ = false; };

	void play_loop(const dpp::voiceconn* v)
	{
		if (v && v->voiceclient && v->voiceclient->is_ready())
		{
			ogg_sync_state oy;
			ogg_stream_state os;
			ogg_page og;
			ogg_packet op;
			OpusHead header;
			char* buffer;

			FILE* fd = nullptr;
			for (uint32_t i = 0; i < 100 && !fd; i++)
			{
				std::this_thread::sleep_for(50ms);
				fd = fopen(std::string("songs/aa"/* + operator[](0).id + */".opus").c_str(), "rb");
				std::cout << "try\n";
			};
			if (!fd)
			{
				std::cout << "File failed\n";
				return;
			};
			std::cout << "File worked\n";

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
					std::cout << "Woo\n";
					v->voiceclient->send_audio_opus(op.packet, op.bytes, samples / 48);
				}
			}
			ogg_stream_clear(&os);
			ogg_sync_clear(&oy);
		};
	};

	const auto begin() { return queue_.begin(); };
	const auto end() { return queue_.end(); };

private:
	std::vector<song> queue_;
	bool playing_ = false;
	bool loop_ = false;
	uint32_t volume_ = 100;
};

per_guild<player> players;
