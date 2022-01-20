#pragma once
#include <vector>
#include <structures/per_guild.hpp>
#include "yt_search.hpp"
#include <utils/clock.hpp>

class player
{
public:
	player(dpp::discord_client* _client, dpp::snowflake _guildId) : client_(_client), guildId_(_guildId) {};
	player(const player& _other) : queue_(_other.queue_), client_(_other.client_), guildId_(_other.guildId_), volume_(_other.volume_), loop_(_other.loop_) {};
	~player() = default;

	const song& operator[](uint32_t _i)
		{ return queue_[_i]; };

	void song_add(const song& _song)
		{ queue_.push_back(_song); append_buffer(); };

	void song_remove(uint32_t _i)
		{ queue_.erase(queue_.begin() + _i); rebuild_buffer(); };

	void songs_swap(uint32_t _i1, uint32_t _i2)
		{ std::swap(queue_[_i1], queue_[_i2]); rebuild_buffer(); };

	void skip() // TODO : change to "jumpto" function instead
		{ voiceConnection_()->voiceclient->skip_to_next_marker(); };

	dpp::snowflake guildId()
		{ return guildId_; };

	uint32_t nSongs()
		{ return queue_.size(); };

	uint32_t length()
		{ uint32_t t = 0; for (auto s : queue_) t += clock_to_time(s.duration); return t; };

	uint32_t volume_set(uint32_t _volume)
		{ volume_ = _volume; rebuild_buffer(); };

	uint32_t volume_get()
		{ return volume_; };

	bool paused()
		{ if(voiceConnection_()) { return voiceConnection_()->voiceclient->is_paused(); }; return true; };

	void resume()
		{ if(voiceConnection_()) { voiceConnection_()->voiceclient->pause_audio(false); }; };

	void pause()
		{ if(voiceConnection_()) { voiceConnection_()->voiceclient->pause_audio(true); }; };
	
	// Flush the audio buffer and make a new one, this needs to be done every time the user changes effects, volume etc.(stupid, but I don't know of any other easy way)
	void rebuild_buffer();
	void append_buffer();

	const auto begin() { return queue_.begin(); };
	const auto end() { return queue_.end(); };

private:
	std::vector<song> queue_;
	dpp::discord_client* client_;
	dpp::snowflake guildId_;
	dpp::voiceconn* voiceConnection_() { return client_->get_voice(guildId_); };
	uint32_t volume_ = 100;
	bool loop_ = false;
};

// List of all the guilds' players
inline per_guild<player> players;