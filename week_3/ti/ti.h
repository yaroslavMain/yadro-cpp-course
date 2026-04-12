#pragma once
#include <string_view>

struct int_tree_t {
    int value;
    std::unique_ptr<int_tree_t> left;
    std::unique_ptr<int_tree_t> right;
};

namespace ti_utils {
    constexpr int parse_int(const std::string_view sv) {
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

    using iter = std::string_view::const_iterator;

    struct parse_state {
        iter pos;
        iter end;

        [[nodiscard]] char peek() const { return pos != end ? *pos : 0; }
        void advance() { if (pos != end) ++pos; }
        void skip_whitespace() { while (pos != end && (*pos == ' ' || *pos == '\t')) ++pos; }
    };
}

std::unique_ptr<int_tree_t> parse_node(ti_utils::parse_state &state) {
    state.skip_whitespace();
    if (state.peek() == '<') {
        state.advance(); // skip '<'
        state.skip_whitespace();

        // Parse left subtree
        std::unique_ptr<int_tree_t> left;
        if (state.peek() != '|') {
            left = parse_node(state);
        }
        state.skip_whitespace();
        assert(state.peek() == '|');
        state.advance(); // skip '|'
        state.skip_whitespace();

        // Parse value
        assert(state.peek() >= '0' && state.peek() <= '9');
        const ti_utils::iter start = state.pos;
        while (state.pos != state.end && (state.peek() >= '0' && state.peek() <= '9')) {
            state.advance();
        }
        const int value = ti_utils::parse_int(std::string_view(start, state.pos - start));
        state.skip_whitespace();
        assert(state.peek() == '|');
        state.advance(); // skip '|'
        state.skip_whitespace();

        // Parse right subtree
        std::unique_ptr<int_tree_t> right;
        if (state.peek() != '>') {
            right = parse_node(state);
        }
        state.skip_whitespace();
        assert(state.peek() == '>');
        state.advance(); // skip '>'

        return std::make_unique<int_tree_t>(int_tree_t{value, std::move(left), std::move(right)});
    }

    const ti_utils::iter start = state.pos;
    while (state.pos != state.end && (state.peek() >= '0' && state.peek() <= '9'))
        state.advance();

    const int value = ti_utils::parse_int(std::string_view(start, state.pos - start));
    return std::make_unique<int_tree_t>(int_tree_t{value, nullptr, nullptr});
}

inline std::unique_ptr<int_tree_t> operator""_ti(const char *s, size_t len) {
    std::string_view sv(s, len);
    ti_utils::parse_state state{sv.begin(), sv.end()};
    return parse_node(state);
}
