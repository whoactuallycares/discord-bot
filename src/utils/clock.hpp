#pragma once
#include <string>

std::string time_to_clock(uint32_t _time)
{
	return std::to_string(_time / 60) + ":" + std::to_string(_time % 60);
};

uint32_t clock_to_time(std::string _clock)
{
	uint32_t delimiter = _clock.find(":");
	uint32_t time = std::stoi(std::string(_clock.begin(), _clock.begin() + delimiter)) * 60;
	return time + std::stoi(std::string(_clock.begin() + delimiter + 1, _clock.end()));
};