#pragma once
#include <vector>
#include <string>
#include <string_view>

struct song
{
	song(std::string_view& _strView, uint32_t _len) : name_(_strView), length_(_len) {};
	std::string name_;
	uint32_t length_;
};

class player
{
public:
	player() {};
	~player() {};

	void song_add(const song& _song) { queue_.push_back(_song); };
	void song_remove(uint32_t _i) { queue_.erase(queue_.begin() + _i); };
	void songs_swap(uint32_t _i1, uint32_t _i2) { std::swap(queue_[_i1], queue_[_i2]); };

	const std::vector<song>::iterator begin() { return queue_.begin(); };
	const std::vector<song>::iterator end() { return queue_.end(); };

	bool playing_ = true;
	bool loop_ = false;
	uint32_t volume_ = 100;
private:
	std::vector<song> queue_;
};
