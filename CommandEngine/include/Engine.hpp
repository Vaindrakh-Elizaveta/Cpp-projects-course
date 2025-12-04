#pragma once
#include <string>
#include <unordered_map>
#include "ICommand.hpp"

class Engine {
public:
	void register_command(ICommand* cmd, const std::string& name);

	ArgValue execute(const std::string& name, const ArgList& args);
	
private:
	std::unordered_map<std::string, ICommand*> commands;
};