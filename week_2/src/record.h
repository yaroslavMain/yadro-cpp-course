#pragma once

struct record_t {
    size_t id{};
    const char *name{};

    record_t() = default;
    record_t(const size_t id, const char *name) : id(id), name(name) {}
};
