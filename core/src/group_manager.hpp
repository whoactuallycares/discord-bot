#pragma once
#include <DPP/dpp.h>

class GroupManager
{
public:
	void add_user_group(const dpp::snowflake _uId, const std::string& _group);
	void remove_user_group(const dpp::snowflake _uId, const std::string& _group);
	const std::vector<std::string>&  user_groups(const dpp::snowflake _uId);
private:
	std::unordered_map<dpp::snowflake, std::vector<std::string>> userGroups_;
};