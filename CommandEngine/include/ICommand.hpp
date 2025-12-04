#pragma once
#include<string>
#include <vector>
#include <variant>
#include <utility>
#include <ostream>
#include <type_traits>

using ArgValue = std::variant<std::monostate, int, double, std::string, bool>;
using ArgList = std::vector<std::pair<std::string, ArgValue>>;

//Удобный вывод ArgValue в std::cout
inline std::ostream& operator<<(std::ostream& os, const ArgValue& v)
{
    std::visit([&](const auto& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            os << "<void>";    //для функций с типом возврата void
        }
        else {
            os << value;
        }
        }, v);
    return os;
}

class ICommand {
public:
    virtual ~ICommand() = default;

    //Каждый класс-команда обязан реализовать этот метод
    virtual ArgValue execute(const ArgList& args) = 0;
};