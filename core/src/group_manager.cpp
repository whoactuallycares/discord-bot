#include "group_manager.hpp"

void GroupManager::add_user_group(const dpp::snowflake _uId, const std::string& _group)
{
	userGroups_[_uId].push_back(_group);
};

void GroupManager::remove_user_group(const dpp::snowflake _uId, const std::string& _group)
{
	const auto it = std::find_if(userGroups_[_uId].begin(), userGroups_[_uId].end(),
		[&_group](const std::string& _userGroup) {
			return _group == _userGroup;
		});
	if (it == userGroups_[_uId].end())
		throw std::exception("User is not a part of this group");

	userGroups_[_uId].erase(it);
};

const std::vector<std::string>& GroupManager::user_groups(const dpp::snowflake _uId)
{
	return userGroups_[_uId];
};
