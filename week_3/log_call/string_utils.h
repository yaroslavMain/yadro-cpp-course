#pragma once

namespace utils {
    constexpr bool str_eq(const char *s1, const char *s2) {
        if (!s1 || !s2) return s1 == s2;
        while (*s1 && (*s1 == *s2)) {
            ++s1;
            ++s2;
        }
        return *s1 == *s2;
    }

    constexpr bool str_in(const char *s, const char *const*begin, const char *const*end) {
        while (begin != end) {
            if (str_eq(s, *begin)) {
                return true;
            }
            begin++;
        }
        return false;
    }

    template<size_t N>
    constexpr bool str_in(const char *const s, char const *const (&list)[N]) {
        return str_in(s, list, list + N);
    }
}
