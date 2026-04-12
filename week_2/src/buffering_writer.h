#pragma once
struct record_t;

class buffering_writer {
    struct Impl;
    std::unique_ptr<Impl> m_pImpl;

    Impl *impl() { return m_pImpl.get(); }
    [[nodiscard]] const Impl *pimpl() const { return m_pImpl.get(); }

    void background_writer();

public:
    explicit buffering_writer(const std::string &file_name);
    ~buffering_writer();

    void write_record(const record_t &record);
    [[nodiscard]] size_t pending_count() const;
};
