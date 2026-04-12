#pragma once
#include "lexer.h"

struct RPN {
    template<size_t N>
    constexpr static int run(const Lexer<N> &lexer) {
        int stack[N]{};
        int top = 0;

        for (size_t i = 0; i < lexer.size(); i++) {
            const auto &token = lexer[i].value;

            if (const int *val = std::get_if<int>(&token)) {
                if (top >= N) throw std::out_of_range("stack overflow");
                stack[top++] = *val;
            } else {
                if (top < 2) throw std::invalid_argument("stack underflow");
                int b = stack[--top];
                int a = stack[--top];

                auto func = std::get<OpAction>(token);
                stack[top++] = func(a, b);
            }
        }
        if (top != 1) throw std::invalid_argument("too many operands in stack");
        return stack[0];
    }
};


constexpr int operator""_rpn(const char *s, const size_t len) {
    std::string_view sv(s, len);
    return RPN::run(Lexer<128>(sv));
}
