#pragma once
#include <dpp/dpp.h>
#include <fstream>
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
			data_.emplace_back(bot_->get_shard(data_.size() / 2500), _guildId); // TODO : Can fail if there isn't enough shards
		};
		return data_[map_[_guildId]];
	};

	void init(dpp::cluster& _bot)
		{ bot_ = &_bot; };

	void serialize(const std::string& _path)
	{
		std::ofstream f(_path, std::ios::binary);
		if (!f.is_open())
		{
			return;
		};
		size_t sMap = map_.size();
		f.write((char*)&sMap, sizeof(sMap));
			std::cout << "h\n";
		for (auto el : map_)
		{
			std::cout << "h\n";
			f.write((char*)&el.first, sizeof(el.first));
			std::cout << "h" << el.second << " a " << sizeof(T) << "\n";
			f.write((char*)&data_[el.second], sizeof(T));
		};
		f.close();
	};

	void deserialize(const std::string& _path)
	{
		std::cout << "deserializing...\n";
		std::ifstream f(_path, std::ios::binary);
		if (!f.is_open())
		{
			return;
		};
		size_t sMap;
		f.read((char*)&sMap, sizeof(sMap));
		for (uint64_t i = 0; i < sMap; i++)
		{
			std::cout << "deserializing...\n";
			dpp::snowflake guildId;
			f.read((char*)&guildId, sizeof(dpp::snowflake));
			T* el = (T*)new char[sizeof(T)];
			f.read((char*)el, sizeof(T));
			std::cout << "deserializing..." << guildId <<"\n";
			map_.insert({guildId, data_.size()});
			data_.emplace_back(*el);
			delete el;
		};
		f.close();
	};
private:
	dpp::cluster* bot_;
	std::map<dpp::snowflake, uint32_t> map_; // Map guildId to an index in data_ vector
	std::vector<T> data_;
};