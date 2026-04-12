#pragma once
#include <array>

using OpAction = int(*)(int, int);

struct OpConfig {
    std::string_view name;
    OpAction op;
};

namespace ops {
    constexpr int add(const int a, const int b) { return a + b; }
    constexpr int sub(const int a, const int b) { return a - b; }
    constexpr int mul(const int a, const int b) { return a * b; }

    constexpr int div(const int a, const int b) {
        if (b == 0) {
            throw std::invalid_argument("division by zero");
        }
        return a / b;
    }

    constexpr int mod(const int a, const int b) { return a % b; }

    constexpr int pow(const int a, const int b) {
        int result = 1;
        for (size_t i = 0; i < b; i++) {
            result *= a;
        }
        return result;
    }
}

struct RPNSettings {
    static constexpr std::array<OpConfig, 6> operations = {
        {
            {"+", ops::add},
            {"-", ops::sub},
            {"*", ops::mul},
            {"/", ops::div},
            {"%", ops::mod},
            {"^", ops::pow}
        }
    };

    static constexpr OpAction find_action(const std::string_view &sw) {
        for (const auto &[name, op]: operations) {
            if (name == sw) {
                return op;
            }
        }
        return nullptr;
    }
};
