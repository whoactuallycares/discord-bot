#pragma once
#include <random>

namespace bot::rand
{
	inline std::random_device rd;
	inline std::mt19937 rng = std::mt19937(rd());

	template<typename T>
	inline T rand(T _min, T _max)
	{
		std::uniform_int_distribution<T> uni(_min, _max);
		return uni(rng);
	};
};
