#include <iostream>
#include <vector>
#include <string>
#include <dpp/dpp.h>
#include <fmt/format.h> // TODO : Replace with ptl::fmt
#include <string_view>
#include "commands.hpp"
#include <thread>
#include <chrono>
#include <token.hpp>
using namespace std::chrono_literals;

#ifdef _WIN32
#include <windows.h>
#define dlopen(name, flags) LoadLibraryA(name)
#define dlclose(handle) FreeLibrary(static_cast<HMODULE>(handle))
#define dlsym(handle, name) GetProcAddress(static_cast<HMODULE>(handle), name)
static_assert(sizeof(void*) == sizeof(HMODULE));
#else
#include <dlfcn.h>
#include <functional>
#endif // _WIN32

std::vector<std::pair<std::string, std::function<void(dpp::cluster&, const dpp::message_create_t&, const std::vector<std::string_view>&)>>> commands = {
	{"info", command_info},
	{"modules", command_modules},
};
uint32_t nModules = 0;

uint32_t getNModules()
{
    return nModules;
}
std::vector<std::pair<std::string, std::function<void(dpp::cluster&, const dpp::message_create_t&, const std::vector<std::string_view>&)>>>& getcommands()
{
    return commands;
};

void* loadmod(const char* _name)
{
	void* hMod  = dlopen(_name, RTLD_NOW);
    if (!hMod)
        return 0;
	//void(*func1)() = (void(*)())dlsym(lib1, "func1");
	//if (!func1)
	//{
	//	std::cout << "lib1 func1 doesn't exist";
	//	return 0;
	//};
	void* modName = dlsym(hMod, "name");
    DWORD dd = GetLastError();
	if (!modName)
		return 0;
	std::cout << "Loaded module : " << modName;
	void* nCommands = dlsym(hMod, "nCommands");
	void* command_list = dlsym(hMod, "commands");
    for (uint32_t i = 0; i < *static_cast<uint32_t*>(nCommands); i++)
    {
        commands.push_back(static_cast<std::pair<std::string, std::function<void(dpp::cluster&, const dpp::message_create_t&, const std::vector<std::string_view>&)>>*>(command_list)[i]);
        std::cout << "Loaded command " << commands[commands.size() - 1].first << "\n";
    };
    nModules++;
	return hMod;
};

void freemod(void* _module)
{
	dlclose(_module);
};

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
    dpp::cluster bot(DISCORD_BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
    //players.init(bot);
    //players.deserialize("test.bin");

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
    //bot.on_typing_start([&bot](const auto& event) {
	//	bot.message_create(dpp::message(event.typing_channel->id, "Watchu typing..."));
	//});
    bot.on_voice_client_disconnect([](const auto& event) { std::cout << "client disconnect\n"; });
    bot.on_voice_state_update([&bot](const auto& event) { 
        std::cout << "state update\n" <<event.state.channel_id << "\n";
        if (event.state.user_id == bot.me.id)
        {
            if (event.state.channel_id == 0)
            {
                //players[event.state.guild_id].pause();
            }
            else
            {
                //std::cout << "Vociceready = " << v.voiceclient->is_ready() << "\n";
                //players[event.state.guild_id].play_loop(&v);
            };
        };
	});
    bot.on_voice_ready([](const dpp::voice_ready_t& event) {
		//dpp::voiceconn* v = event.from->get_voice(event.voice_client->server_id);
		//std::cout << "Vociceclient = " << v->voiceclient << "\n";
	});
    bot.on_voice_user_talking([](const auto& event) { std::cout << "user talking\n"; });
    bot.on_voice_client_speaking([](const auto& event) { std::cout << "client speaking\n"; });
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

    bot.on_message_create([&bot](const auto& _event) {
        // Make sure the message is a valid command
        const std::string& content = _event.msg.content;
        if (content.size() <= 2)
            return;
        if (content[0] != '\'')
            return;
        const auto parsed = command_parse(content);
        const auto it = std::find_if(commands.begin(), commands.end(),
            [&parsed](const std::pair <std::string, std::function<void(dpp::cluster&, const dpp::message_create_t&, const std::vector<std::string_view>&)>>& _command)
            { return _command.first == parsed[0]; }
        );
        if (it == commands.end())
        {
            std::string cmdName{ parsed[0].data(), parsed[0].size() };
            bot.message_create(dpp::message(_event.msg.channel_id, "Unknown command `" + cmdName + "`"));
            return;
        };
        (*it).second(bot, _event, parsed);
        //select_command(bot, _event, command_parse(_event.msg.content));
	});

    bot.on_log([](const dpp::log_t& log) {std::cout << log.message << "\n"; });

    bot.start(false);
    return 0;
};