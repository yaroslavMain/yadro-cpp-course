#pragma once
#include <vector>
#include <string_view>
#include <charconv>
#include <stdexcept>

namespace vector_config {
    constexpr char separators[] = {',', ';', ':', '=', ' ', '_', '.'};

    constexpr bool is_separator(const char s) {
        for (const auto c: separators) {
            if (c == s) return true;
        }
        return false;
    }
}

namespace vi_utils {
    constexpr int parse_int(const std::string_view sv) {
        int value = 0;
        auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);

        if (ec == std::errc::result_out_of_range)
            throw std::out_of_range("too big");
        if (ec == std::errc::invalid_argument || ptr != sv.data() + sv.size())
            throw std::invalid_argument("invalid value or trailing characters");

        return value;
    }
}

constexpr std::string_view get_next_token(std::string_view &sv) {
    // Finding start
    const auto start = std::find_if_not(sv.begin(), sv.end(), vector_config::is_separator);
    if (start == sv.end()) return "";
    sv.remove_prefix(std::distance(sv.begin(), start));

    // Finding end
    const auto end = std::find_if(sv.begin(), sv.end(), vector_config::is_separator);

    // Create token and substring sv
    const std::string_view token = sv.substr(0, std::distance(sv.begin(), end));
    sv.remove_prefix(token.size());

    return token;
}

std::vector<int> operator""_vi(const char *s, const size_t len) {
    std::string_view sv(s, len);
    std::vector<int> result;

    while (!sv.empty()) {
        const std::string_view token = get_next_token(sv);
        if (token.empty()) break;
        result.push_back(vi_utils::parse_int(token));
    }
    return result;
}
