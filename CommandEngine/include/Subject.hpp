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
};