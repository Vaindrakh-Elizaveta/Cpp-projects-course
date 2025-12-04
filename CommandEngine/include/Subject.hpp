#pragma once
#include <string>
#include <iostream>

class Subject {
public:
    // 1) Простая сумма двух int
    int sum(int a, int b) {
        std::cout << "[Subject::sum] a = " << a << ", b = " << b << "\n";
        return a + b;
    }

    // 2) Взвешенная сумма трёх double
    double weighted_sum(double a, double b, double w) {
        std::cout << "[Subject::weighted_sum] a = " << a
            << ", b = " << b << ", w = " << w << "\n";
        return a * w + b * (1.0 - w);
    }

    // 3) Повтор строки n раз
    std::string repeat(const std::string s, int n) {
        std::cout << "[Subject::repeat] s = \"" << s << "\", n = " << n << "\n";
        std::string res;
        for (int i = 0; i < n; ++i)
            res += s;
        return res;
    }

    // 4) Функция без параметров
    int get_magic_number() {
        std::cout << "[Subject::get_magic_number]\n";
        return 42;
    }

    // 5) Возвращает строку в зависимости от булевского флага
    std::string yes_no(bool flag) {
        std::cout << "[Subject::yes_no] flag = " << flag << "\n";
        return flag ? "YES" : "NO";
    }

    // 6) Склеивает строку и число
    std::string mix(std::string s, double x) {
        std::cout << "[Subject::mix] s = \"" << s << "\", x = " << x << "\n";
        return s + " * " + std::to_string(x);
    }

    // 7) Печатает три числа, ничего не возвращает
    void log3(int a, int b, int c) {
        std::cout << "[Subject::log3] a = " << a
            << ", b = " << b
            << ", c = " << c << "\n";
    }

    // 8) Вычисляет линейную комбинацию 5 чисел
    double linear5(double a, double b, double c, double d, double e) {
        std::cout << "[Subject::linear5] called\n";
        return a + 2 * b + 3 * c + 4 * d + 5 * e;
    }
};