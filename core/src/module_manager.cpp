#include "module_manager.hpp"
#include <ranges>

void CommandRegistry::register_module(const std::string& _name)
{
	// Load module
	void* hMod  = dlopen(_name.c_str(), RTLD_NOW);
    if (!hMod)
        throw std::exception("Failed to load module");

	// Get module info
	module_header* pModInfo = reinterpret_cast<module_header*>(dlsym(hMod, "modInfo"));
	if (!pModInfo)
        throw std::exception("Failed to read module data, module might be malformed");

	auto& modInfo = modules_.emplace_back(*pModInfo);
	modInfo.handle = hMod;
	modInfo.iFirstCommand = commands_.size();

	// Register commands
	command_header* pCommands = reinterpret_cast<command_header*>(dlsym(hMod, "commands"));

    for (uint32_t i = 0; i < modInfo.nCommands; i++)
		register_command(pCommands[i]);
};

void CommandRegistry::unregister_module(const std::string& _name)
{
	// Find module to remove
	const auto it = std::ranges::find_if(modules_,
		[&_name](const module_header& _modInfo) {
			return !std::strcmp(_name.c_str(), _modInfo.name);
		});
	if (it == modules_.end())
		throw std::exception("Unknown module");
	auto& modInfo = *it;

	// Unregister commands
	commands_.erase(commands_.begin() + modInfo.iFirstCommand, commands_.begin() + modInfo.nCommands - 1);

	// Unregister module
	dlclose(modInfo.handle);
};

const command_function& CommandRegistry::find(const std::string& _name)
{
	// Try to find command by name
	const auto it = std::ranges::find_if(commands_,
		[&_name](const command_header& _command) {
			return _command.name == _name;
		});

	if (it == commands_.end())
		return 0;

	return (*it).func;
};

void CommandRegistry::register_command(const command_header& _cmdInfo)
{
	// Add command to registry
	commands_.emplace_back(_cmdInfo);
	std::cout << "Registered command : " << _cmdInfo.name << "\n";
};