#pragma once
#include "log_config.h"
#include "string_utils.h"

#define LOG_CALL logger::log_call(__FUNCTION__)

namespace logger {
    constexpr void log_call(const char *function) {
        if (utils::str_in(function, log_config::interesting)) {
            std::cout << "[LOG]: " << function << std::endl;
        }
    }
}
