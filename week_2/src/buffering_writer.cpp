#include <list>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>
#include <sys/fcntl.h>
#include "record.h"
#include "buffering_writer.h"

struct buffering_writer::Impl {
    std::mutex mutex;
    std::string m_filename;
    int m_file = -1;
    std::vector<record_t> m_buffer;

    // Thread
    std::thread m_flusher;
    bool m_is_worker_running = true;
    std::condition_variable m_condition;

    explicit Impl(std::string filename): m_filename(std::move(filename)) {
        m_file = open(m_filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (m_file == -1) {
            throw std::runtime_error("Failed to open file for writing");
        }
        assert(m_file >= 0);
    }

    Impl(Impl const &other) = delete;
    Impl &operator=(Impl const &other) = delete;

    ~Impl() { if (m_file >= 0) close(m_file); }
};

buffering_writer::buffering_writer(const std::string &file_name): m_pImpl(std::make_unique<Impl>(file_name)) {
    impl()->m_flusher = std::thread(&buffering_writer::background_writer, this);
}

buffering_writer::~buffering_writer() {
    // Thread
    {
        std::unique_lock lock(m_pImpl->mutex);
        impl()->m_is_worker_running = false;
    }
    impl()->m_condition.notify_one();

    assert(impl()->m_flusher.joinable());
    if (impl()->m_flusher.joinable())
        impl()->m_flusher.join();
    assert(m_pImpl->m_buffer.empty());
}


void buffering_writer::background_writer() {
    while (true) {
        std::vector<record_t> buffer;

        // Fast operations
        {
            std::unique_lock lock(m_pImpl->mutex);
            impl()->m_condition.wait(lock, [this] {
                return !pimpl()->m_buffer.empty() || !pimpl()->m_is_worker_running;
            });

            if (!pimpl()->m_is_worker_running && pimpl()->m_buffer.empty()) break;
            buffer.swap(impl()->m_buffer); // double buffering
            assert(m_pImpl->m_buffer.empty());
        }

        // Slow operations
        if (!buffer.empty()) {
            assert(m_pImpl->m_file >= 0);

            const size_t written_bytes = write(pimpl()->m_file, buffer.data(), sizeof(record_t) * buffer.size());
            assert(written_bytes == sizeof(record_t) * buffer.size());
        }
    }
}

void buffering_writer::write_record(const record_t &record) {
    {
        std::unique_lock lock(m_pImpl->mutex);
        assert(pimpl()->m_is_worker_running);
        impl()->m_buffer.push_back(record);
    }
    impl()->m_condition.notify_one(); // only background_writer
}

size_t buffering_writer::pending_count() const {
    std::unique_lock lock(m_pImpl->mutex);
    return pimpl()->m_buffer.size();
}
