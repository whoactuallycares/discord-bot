#pragma once
#include <random>

class random
{
public:
	static int64_t randint(int64_t _min, int64_t _max)
	{
		std::uniform_int_distribution<int> uni(_min, _max);
		return uni(_rng);
		auto random_integer = uni(_rng);
	};
	static std::random_device _rd;
	static std::mt19937 _rng;
};

std::random_device random::_rd;
std::mt19937 random::_rng = std::mt19937(_rd());