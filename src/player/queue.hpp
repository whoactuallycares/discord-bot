#pragma once
#include "song.hpp"
#include <vector>


class queue
{
public:
	queue() {};
	~queue() {};

	void song_add(const song& _song) { songs_.push_back(_song); };
	void song_remove(uint32_t _i) { songs_.erase(songs_.begin() + _i); };
	void songs_swap(uint32_t _i1, uint32_t _i2) { std::swap(songs_[_i1], songs_[_i2]); };

	const std::vector<song>::iterator begin() { return songs_.begin(); };
	const std::vector<song>::iterator end() { return songs_.end(); };

	bool playing_ = true;
	bool loop_ = false;
	uint32_t volume_ = 100;
private:
	std::vector<song> songs_;
};