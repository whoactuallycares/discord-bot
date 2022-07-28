#pragma once
#include <functional>
#include <dpp/dpp.h>
#include <headers.hpp>

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

class CommandRegistry
{
public:
	void register_module(const std::string& _name);
	void unregister_module(const std::string& _name);
	void register_command(const command_header& _cmdInfo);
	const command_function& find(const std::string& _name);

	uint32_t nModules() { return modules_.size(); };
	uint32_t nCommands() { return commands_.size(); };

private:

	std::vector<module_header> modules_;
	std::vector<command_header> commands_;
};

//std::vector<std::pair<std::string, command_function>> commands = {
//	{"info", command_info},
//	{"modules", command_modules},
//};
//uint32_t nModules = 0;
//
//uint32_t getNModules()
//{
//    return nModules;
//}
//std::vector<std::pair<std::string, command_function>>& getcommands()
//{
//    return commands;
//};
//
//void* loadmod(const char* _name)
//{
//	void* hMod  = dlopen(_name, RTLD_NOW);
//    if (!hMod)
//        return 0;
//	//void(*func1)() = (void(*)())dlsym(lib1, "func1");
//	//if (!func1)
//	//{
//	//	std::cout << "lib1 func1 doesn't exist";
//	//	return 0;
//	//};
//	void* modName = dlsym(hMod, "name");
//    DWORD dd = GetLastError();
//	if (!modName)
//		return 0;
//	std::cout << "Loaded module : " << modName;
//	void* nCommands = dlsym(hMod, "nCommands");
//	void* command_list = dlsym(hMod, "commands");
//    for (uint32_t i = 0; i < *static_cast<uint32_t*>(nCommands); i++)
//    {
//        commands.push_back(static_cast<std::pair<std::string, command_function>*>(command_list)[i]);
//        std::cout << "Loaded command " << commands[commands.size() - 1].first << "\n";
//    };
//    nModules++;
//	return hMod;
//};
//
//void freemod(void* _module)
//{
//	dlclose(_module);
//};