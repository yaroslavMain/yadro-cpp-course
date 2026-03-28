#pragma once
#include <algorithm>

class bitset {
    static constexpr size_t bytes_per_word = sizeof(uint64_t);
    static constexpr size_t bits_per_word = bytes_per_word * 8;

    uint64_t *data_ = nullptr;
    size_t capacity_ = 0; // in bits

    [[nodiscard]] static size_t words_for_bits(size_t bits) noexcept;
    [[nodiscard]] static size_t block_index(size_t pos) noexcept;
    [[nodiscard]] static size_t bit_mask(size_t pos) noexcept;
    [[nodiscard]] size_t word_capacity_count() const noexcept;

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
    void clear() noexcept;

    [[nodiscard]] bitset union_with(const bitset &other) const;
    [[nodiscard]] bitset intersection(const bitset &other) const;
    [[nodiscard]] bool is_subset(const bitset &other) const;
};
