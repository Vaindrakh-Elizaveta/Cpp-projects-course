#pragma once
#include <vector>
#include <string>
#include <variant>
#include <utility>

using ArgValue = std::variant<std::monostate, int, double, std::string, bool>;
using ArgList = std::vector<std::pair<std::string, ArgValue>>;

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual ArgValue execute(const ArgList& args) = 0;
};
