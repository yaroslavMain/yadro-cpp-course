#pragma once
#include <string_view>
#include "config.h"

struct Token {
    std::variant<OpAction, int> value;
};

template<size_t MaxTokens = 64>
class Lexer {
    std::array<Token, MaxTokens> tokens;
    size_t count = 0;

    static constexpr int parse_int(const std::string_view sv) {
        if (sv.empty()) throw std::invalid_argument("Empty token");

        int value = 0;
        size_t i = 0;
        bool negative = false;

        if (sv[0] == '-') {
            i = 1;
            negative = true;

            if (sv.size() == 1)
                throw std::invalid_argument("Invalid number: just a minus");
        }

        for (; i < sv.size(); ++i) {
            if (sv[i] < '0' || sv[i] > '9')
                throw std::invalid_argument("invalid character in number");

            const int digit = sv[i] - '0';
            value = value * 10 + digit;
        }

        return negative ? -value : value;
    }

public:
    constexpr explicit Lexer(std::string_view &sw) {
        size_t start = 0;
        while (start < sw.size()) {
            if (sw[start] == ' ') {
                start++;
                continue;
            }
            size_t end = sw.find(' ', start);
            if (end == std::string_view::npos) { end = sw.size(); }

            assert(end > start);
            std::string_view token = sw.substr(start, end - start);
            start = end + 1;

            // Add new action to array
            if (const auto &action = RPNSettings::find_action(token); action != nullptr) {
                tokens[count++] = Token{.value = action};
            } else {
                tokens[count++] = Token{.value = parse_int(token)};
            }
            assert(!tokens.empty());
        }
    }

    constexpr Token operator[](size_t i) const { return tokens[i]; }
    [[nodiscard]] constexpr size_t size() const { return count; }
};
