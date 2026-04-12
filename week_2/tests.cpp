#include <fstream>
#include <thread>
#include <gtest/gtest.h>

#include "src/record.h"
#include "src/buffering_writer.h"

void remove_file(const std::string &filename) {
    std::remove(filename.c_str());
}

inline void assert_number_of_lines(const std::string &file_nane, const size_t number_of_lines,
                                   const bool is_inner_check = true) {
    std::ifstream in_file(file_nane);
    ASSERT_TRUE(in_file.is_open());

    int count = 0;
    record_t read_rec;
    while (in_file.read(reinterpret_cast<char *>(&read_rec), sizeof(record_t))) {
        if (is_inner_check) {
            ASSERT_EQ(read_rec.id, count);
        }
        count++;
    }
    ASSERT_EQ(count, number_of_lines);
}

TEST(BufferingWriteTests, Simple) {
    const std::string filename = "test.bin";
    remove_file(filename); {
        buffering_writer w(filename);
        for (size_t id = 0; id < 1000; ++id) {
            record_t rec{id, "Test"};
            w.write_record(rec);
        }
    }

    assert_number_of_lines(filename, 1000);
}

TEST(BufferingWriteTests, Complex) {
    const std::string filename = "test.bin";

    constexpr size_t number_of_threads = 10;
    constexpr size_t records_per_thread = 1000;

    remove_file(filename); {
        buffering_writer w(filename);
        std::vector<std::thread> threads;
        threads.reserve(number_of_threads);

        for (size_t id = 0; id < number_of_threads; ++id) {
            threads.emplace_back([&w, records_per_thread]() {
                for (size_t i = 0; i < records_per_thread; ++i) {
                    record_t rec{i, "Test"};
                    w.write_record(rec);
                }
            });
        }

        for (auto &thread: threads) {
            thread.join();
        }
    }

    assert_number_of_lines(filename, number_of_threads * records_per_thread, false);
}
