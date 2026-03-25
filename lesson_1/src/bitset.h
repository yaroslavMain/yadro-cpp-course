#pragma once
#include <algorithm>

class bitset {
    uint64_t *data = nullptr;
    size_t bits_count = 0;

    [[nodiscard]] size_t current_block_size() const;
    void resize(size_t new_size);

public:
    bitset();
    explicit bitset(size_t initial_capacity);
    ~bitset();

    bitset(const bitset &other);
    bitset &operator=(const bitset &other);

    bitset(bitset &&other) noexcept;
    bitset &operator=(bitset &&other) noexcept;

    void set(size_t k, bool b);
    [[nodiscard]] bool test(size_t k) const;
    bool operator[](size_t k) const;

    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool empty() const;
    void clear() const;

    [[nodiscard]] bitset union_with(const bitset &other) const;
    [[nodiscard]] bitset intersection(const bitset &other) const;
    [[nodiscard]] bool is_subset(const bitset &other) const;
};
