#pragma once
#include <string>
#include <thread>
#define YT_DLP_PATH "yt-dlp"

struct song
{
	song(const std::string& _url, bool _search = false)
	{
		int64_t rand = random::randint(0, 1000000);
		if (_search)
		{
			system(std::string(YT_DLP_PATH " \"ytsearch:" + _url + "\" --get-title --get-id --get-thumbnail --get-duration > " + std::to_string(rand)).c_str());
		}
		else
		{
			system(std::string(YT_DLP_PATH " \"" + _url + "\" --get-title --get-id --get-thumbnail --get-duration > " + std::to_string(rand)).c_str());
		};

		std::ifstream file(std::to_string(rand), std::ios::binary);
		std::getline(file, title_);
		std::getline(file, id_);
		std::getline(file, thumbnail_);
		std::getline(file, duration_);
		url_ = _search ? "https://www.youtube.com/watch?v=" + id_ : _url;
		file.close();
		std::remove(std::to_string(rand).c_str());
	};
	song(std::string&& _title, std::string&& _id, std::string&& _thumbnail, std::string&& _duration)
		:
		title_(std::move(_title)), id_(std::move(_id)), thumbnail_(std::move(_thumbnail)), duration_(std::move(_duration))
	{};
	std::string url_;
	std::string title_;
	std::string id_;
	std::string thumbnail_;
	std::string duration_;
};

song yt_dlp_url(const std::string& _url)
{
	song s(_url);
	std::thread t([s, _url]() {
		system(std::string(YT_DLP_PATH " --extract-audio --audio-format opus -o \"songs/" + s.id_ + ".opus\" " + _url).c_str());
	});
	t.detach();
	return std::move(s);
};

song yt_dlp_search(const std::string& _name)
{
	song s(_name, true);
	std::thread t([s]() {
		system(std::string(YT_DLP_PATH " --extract-audio --audio-format opus -o \"songs/" + s.id_ + ".opus\" " + s.url_).c_str());
	});
	t.detach();
	return std::move(s);
};
