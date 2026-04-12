#include <gtest/gtest.h>
#include "log_call.h"

TEST(LogCallTests, StrEQ) {
    constexpr auto const *s1 = "hello world";
    constexpr auto *s2 = "hello world";
    constexpr auto *s3 = "world";

    static_assert(utils::str_eq(s1, s2));
    static_assert(!utils::str_eq(s1, s3));
    EXPECT_TRUE(utils::str_eq(s1, s2));
    EXPECT_FALSE(utils::str_eq(s1, s3));
}

TEST(LogCallTests, StrInArray) {
    constexpr const char *list[] = {"init", "open", "close"};

    static_assert(utils::str_in("open", list, list + 3));
    static_assert(!utils::str_in("read", list, list + 3));
    static_assert(utils::str_in("init", list));
    static_assert(!utils::str_in("write", list));

    EXPECT_TRUE(utils::str_in("close", list));
    EXPECT_FALSE(utils::str_in("unknown", list));
}

TEST(LogCallTests, EdgeCases) {
    constexpr const char *empty_list[] = {nullptr};

    EXPECT_FALSE(utils::str_in("test", empty_list, empty_list));
    EXPECT_TRUE(utils::str_eq("", ""));
}

TEST(LogCallTests, LogCallOutput) {
    std::streambuf *old = std::cout.rdbuf();
    const std::stringstream ss;
    std::cout.rdbuf(ss.rdbuf());
    logger::log_call("open");
    logger::log_call("smth");
    std::string output = ss.str();

    EXPECT_NE(output.find("[LOG]: open"), std::string::npos);
    EXPECT_EQ(output.find("smth"), std::string::npos);
    std::cout.rdbuf(old);
}
