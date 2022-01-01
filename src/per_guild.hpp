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
	per_guild();
	per_guild(const per_guild&) = delete;
	per_guild& operator=(const per_guild&) = delete;

	T& operator[](dpp::snowflake _guildId)
	{
		return data_[map_[_guildId]];
	};

	void deserialize(std::string _path);
	void serialize();
private:
	std::map<dpp::snowflake, uint32_t> map_; // Map guildId to an index in data_ vector
	std::vector<T> data_;
};