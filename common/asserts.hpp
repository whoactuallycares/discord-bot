#pragma once
#include <DPP/dpp.h>

inline bool has_mentions(const dpp::message& _message, uint32_t _min = 1)
{
	return _message.mentions.size() >= _min;
};