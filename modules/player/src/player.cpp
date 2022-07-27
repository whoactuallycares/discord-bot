#include "player.hpp"
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ogg/ogg.h>
#include <opusfile.h>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;


void player::rebuild_buffer()
{
	dpp::voiceconn* v = client_->get_voice(guildId_);
    std::cout << "Cumming\n";
    std::cout << "Vociceclient = " << v->voiceclient << "\n";
    std::cout << "Vociceready = " << v->voiceclient->is_ready() << "\n";
    if (v->voiceclient && v->voiceclient->is_ready())
    {
    std::cout << "Cumming\n";
        v->voiceclient->stop_audio();
        v->voiceclient->pause_audio(0);
        ogg_sync_state oy;
        ogg_stream_state os;
        ogg_page og;
        ogg_packet op;
        OpusHead header;
        char* buffer;

        FILE* fd = nullptr;
            std::this_thread::sleep_for(3000ms);
        for (uint32_t i = 0; i < 100 && !fd; i++)
        {
            std::this_thread::sleep_for(50ms);
            fd = fopen(std::string("songs/" + operator[](0).id + ".opus").c_str(), "rb");
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
        v->voiceclient->insert_marker("end");
        ogg_stream_clear(&os);
        ogg_sync_clear(&oy);
    };
};

void player::append_buffer()
{
    rebuild_buffer();
}