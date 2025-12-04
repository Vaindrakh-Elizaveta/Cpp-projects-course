#include "Engine.hpp"
#include <stdexcept>

void Engine::register_command(ICommand* cmd, const std::string& name)
{
    commands[name] = cmd;
}

ArgValue Engine::execute(const std::string& name, const ArgList& args)
{
    auto it = commands.find(name);

    if (it == commands.end()) {
        throw std::runtime_error("Command not found: " + name);
    }

    return it->second->execute(args);
}