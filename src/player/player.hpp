#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <utils/random.hpp>
#include <utils/clock.hpp>
#include "yt_search.hpp"

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

	const auto begin() { return queue_.begin(); };
	const auto end() { return queue_.end(); };

	bool playing = true;
	bool loop = false;
	uint32_t volume = 100;
	dpp::voiceconn* voiceConnection;
private:
	std::vector<song> queue_;
};

void play_loop()
{
};
