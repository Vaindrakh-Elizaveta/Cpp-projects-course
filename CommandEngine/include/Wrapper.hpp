#pragma once
#include "ICommand.hpp"
#include <tuple>
#include <type_traits>
#include <algorithm>
#include <iostream>

template<typename Class, typename R, typename... Args>
class Wrapper : public ICommand {
public:
	using MethodPtr = R(Class::*)(Args...);

    Wrapper(Class* obj, MethodPtr method, const ArgList& defaults)
        : obj_(obj), method_(method), defaults_(defaults)
    {
    }

    ArgValue execute(const ArgList& args) override
    {
        //начинаем с аргументов по умолчанию
        ArgList finalArgs = defaults_;

        //накладываем реальные аргументы по имени
        for (const auto& [name, value] : args) {
            auto it = std::find_if(
                finalArgs.begin(), finalArgs.end(),
                [&](const auto& p) { return p.first == name; }
            );

            if (it != finalArgs.end()) {
                it->second = value; //обновляем значение
            }
            else {
                finalArgs.emplace_back(name, value); //добавляем новый аргумент (опционально)
            }
        }

        //Проверим, хватает ли аргументов
        constexpr std::size_t expected = sizeof...(Args);
        if (finalArgs.size() < expected) {
            throw std::runtime_error("Not enough arguments for command");
        }

        //Вызов реального метода
        if constexpr (std::is_same_v<R, void>) {
            call_with_indices(finalArgs, std::make_index_sequence<expected>{});
            return ArgValue(std::monostate{});  //для void возвращаем пустое значение
        }
        else {
            R result = call_with_indices(finalArgs, std::make_index_sequence<expected>{});
            return ArgValue(result);
        }
    }

private:

    // Достаём значение нужного типа из ArgValue
    // Предполагаем, что T присутствует среди альтернатив ArgValue.
    template<typename T>
    static T get_as(const ArgValue& value)
    {
        return std::get<T>(value);  // бросит std::bad_variant_access, если тип не совпадает
    }

    // Распаковка параметров из finalArgs[I].second в (Args...)
    template<std::size_t... I>
    R call_with_indices(const ArgList& finalArgs, std::index_sequence<I...>)
    {
        return (obj_->*method_)( get_as<Args>(finalArgs[I].second)... );
    }

    Class* obj_;
    MethodPtr method_;
    ArgList defaults_;
};