#pragma once
#include <dpp/dpp.h>
#include <string>
#include <vector>
#include <map>

// A container to store unique variables for each guild the bot serves
template<typename T>
class per_guild
{
public:
	per_guild() = default;
	per_guild(const per_guild&) = delete;
	per_guild& operator=(const per_guild&) = delete;

	inline T& operator[](dpp::snowflake _guildId)
	{
		if (map_.find(_guildId) == map_.end())
		{
			map_.insert({_guildId, data_.size()});
			data_.emplace_back();
		};
		return data_[map_[_guildId]];
	};

	void deserialize()
	{
	};
	void serialize()
	{
		// save the data into "data/guildData"
	};
private:
	std::map<dpp::snowflake, uint32_t> map_; // Map guildId to an index in data_ vector
	std::vector<T> data_;
};