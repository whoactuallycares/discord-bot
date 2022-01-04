#pragma once
#include <random>

namespace rand
{
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());

	template<typename T>
	T rand(T _min, T _max)
	{
		std::uniform_int_distribution<T> uni(_min, _max);
		return uni(rng);
	};
};
