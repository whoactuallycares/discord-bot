#include <iostream>
#include <vector>
#include <string>
#include <dpp/dpp.h>
#include <dpp/fmt/format.h>
#include "player/player.hpp"
//#include "per_guild.hpp"
#include <select_command.hpp>
#include <string_view>
#include <player/yt_search.hpp>

std::vector<std::string_view> command_parse(std::string_view _msg)
{
    std::vector<std::string_view> args;
    uint32_t end = 1, beg;
    while (_msg.size() != end)
    {
        beg = end;
		while (_msg.size() >= end + 1 && (_msg[end] != ' ')) end++;
		args.push_back(_msg.substr(beg, end - beg));
		while (_msg.size() >= end + 1 && (_msg[end] == ' ')) end++;
    };
    return args;
};

// Find a string in an array of strings that begins with this string
uint32_t find(std::string _str, std::vector<std::string> _strs)
{
    uint32_t i = 0;
    uint32_t j = 0;
    start:
    if (_str[j] == _strs[i][j])
    {
        if (_str[j] == _strs[i + 1][j])
        {
            // ambiguous
            if (j + 1 < _str.size())
                j++;
            else
                return ~0; // either ambiguous or wrong
            goto start;
        }
        else
        {
            //it's this one
            return i; // either right or wrong
        };
    }
    else
    {
		if (i + 1 < _strs.size())
			i++;
		else
			return ~0; // didnt find
        goto start;
    };

    while (true)
    {
		if (_str[j] == _strs[i][j])
		{
			if (_str[j] == _strs[i + 1][j])
			{
				if (j + 1 < _str.size())
					j++;
				else
					return ~0; // Ambiguous
			}
			else
			{
                while ((_str[j] && _strs[i][j]) && (_str[j++] == _strs[i][j]));
				return !_str[j] ? i : ~0; // This one or pattern doesn't match
			};
		}
		else
		{
			if (i + 1 < _strs.size())
				i++;
			else
				return ~0; // Didn't find
		};
    };
};

int main()
{
    // Create the bot
    dpp::cluster bot("@TOKEN@");

     //The interaction create event is fired when someone issues your commands 
    bot.on_interaction_create([&bot](const dpp::interaction_create_t& event) {
        if (event.command.type == dpp::it_application_command) {
            dpp::command_interaction cmd_data = std::get<dpp::command_interaction>(event.command.data);
            // Check which command they ran
            if (cmd_data.name == "blep") {
                // Fetch a parameter value from the command parameters
                std::string animal = std::get<std::string>(event.get_parameter("animal"));
                // Reply to the command. There is an overloaded version of this
                // call that accepts a dpp::message so you can send embeds.

                event.reply(dpp::ir_channel_message_with_source, fmt::format("Blep! You chose {}", animal));
            }
        }
	});
    bot.on_voice_ready([](const auto& event) { std::cout << "voice ready"; });
    bot.on_voice_client_disconnect([](const auto& event) { std::cout << "client disconnect"; });

    bot.on_ready([&bot](const auto& event) {
        std::cout << "Logged in as " << bot.me.username << "!\n";


        dpp::slashcommand newcommand;
        // Create a new global command on ready event
        newcommand.set_name("blep")
            .set_description("Send a random adorable animal photo")
            .set_application_id(bot.me.id)
            .add_option(
                dpp::command_option(dpp::co_string, "animal", "The type of animal", true).
                add_choice(dpp::command_option_choice("Dog", std::string("animal_dog"))).
                add_choice(dpp::command_option_choice("Cat", std::string("animal_cat"))).
                add_choice(dpp::command_option_choice("Penguin", std::string("animal_penguin")
                )
                )
            );

        // Register the command
        bot.global_command_create(newcommand);
	});

    bot.on_message_create([&bot](const auto& event) {
        if (event.msg.content[0] != '!')
        {
            return;
        };
        select_command(bot, event, command_parse(event.msg.content));
	});

    bot.start(false);
    return 0;
};