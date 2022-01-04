#pragma once
#include <string>
#include <thread>
#define YT_DLP_PATH "yt-dlp"

struct song
{
	song(const std::string& _url, bool _search = false)
	{
		int64_t rand = bot::rand::rand<int64_t>(0, 1000000);
		if (_search)
		{
			system(std::string(YT_DLP_PATH " \"ytsearch:" + _url + "\" --get-title --get-id --get-thumbnail --get-duration > " + std::to_string(rand)).c_str());
		}
		else
		{
			system(std::string(YT_DLP_PATH " \"" + _url + "\" --get-title --get-id --get-thumbnail --get-duration > " + std::to_string(rand)).c_str());
		};

		std::ifstream file(std::to_string(rand), std::ios::binary);
		std::getline(file, title);
		std::getline(file, id);
		std::getline(file, thumbnail);
		std::getline(file, duration);
		url = _search ? "https://www.youtube.com/watch?v=" + id : _url;
		file.close();
		std::remove(std::to_string(rand).c_str());
	};
	song(const std::string& _title, const std::string& _id, const std::string& _thumbnail, const std::string& _duration)
		:
		title(_title), id(_id), thumbnail(_thumbnail), duration(_duration)
	{};
	std::string url;
	std::string title;
	std::string id;
	std::string thumbnail;
	std::string duration;
};

song yt_dlp_url(const std::string& _url)
{
	song s(_url);
	std::thread t([s, _url]() {
		system(std::string(YT_DLP_PATH " --extract-audio --audio-format opus -o \"songs/" + s.id + ".opus\" " + _url).c_str());
	});
	t.detach();
	return s;
};

song yt_dlp_search(const std::string& _name)
{
	song s(_name, true);
	std::thread t([s]() {
		system(std::string(YT_DLP_PATH " --extract-audio --audio-format opus -o \"songs/" + s.id + ".opus\" " + s.url).c_str());
	});
	t.detach();
	return s;
};
