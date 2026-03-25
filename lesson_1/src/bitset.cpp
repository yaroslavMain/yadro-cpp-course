#include "bitset.h"


size_t bitset::current_block_size() const {
    return (bits_count + 63) / 64;
}

void bitset::resize(const size_t new_size) {
    const size_t old_words = current_block_size();
    const size_t new_words = (new_size + 63) / 64;

    auto *new_data = new uint64_t[new_words]();

    assert(new_data != nullptr);

    if (data != nullptr) {
        std::memcpy(new_data, data, sizeof(uint64_t) * std::min(old_words, new_words));
        delete[] data;
    }

    data = new_data;
    bits_count = new_words * 64;
}

bitset::bitset() = default;

bitset::bitset(const size_t initial_capacity) {
    if (initial_capacity > 0)
        resize(initial_capacity);
}

bitset::~bitset() {
    delete[] data;
}

bitset::bitset(const bitset &other) : bits_count(other.bits_count) {
    const size_t words = current_block_size();
    if (words > 0 && other.data != nullptr) {
        data = new uint64_t[words];
        assert(data != nullptr);
        std::memcpy(data, other.data, sizeof(uint64_t) * words);
    } else {
        data = nullptr;
    }
}

bitset &bitset::operator=(const bitset &other) {
    if (this == &other) return *this;

    delete[] data;
    data = nullptr;

    bits_count = other.bits_count;
    const size_t words = current_block_size();
    if (words > 0 && other.data != nullptr) {
        data = new uint64_t[words];
        assert(data != nullptr);
        std::memcpy(data, other.data, sizeof(uint64_t) * words);
    }

    return *this;
}

bitset::bitset(bitset &&other) noexcept : data(other.data), bits_count(other.bits_count) {
    other.data = nullptr;
    other.bits_count = 0;
}

bitset &bitset::operator=(bitset &&other) noexcept {
    if (this == &other) return *this;
    delete[] data;

    bits_count = other.bits_count;
    data = other.data;

    other.data = nullptr;
    other.bits_count = 0;

    return *this;
}

void bitset::set(const size_t k, const bool b) {
    if (b) {
        if (k >= bits_count)
            resize(std::max(k + 1, bits_count * 2));
        data[k / 64] |= (1ull << (k % 64));
    } else if (k < bits_count) {
        data[k / 64] &= ~(1ull << (k % 64));
    }
}

bool bitset::test(const size_t k) const {
    if (k >= bits_count) return false;
    return data[k / 64] & (1ull << (k % 64));
}

bool bitset::operator[](const size_t k) const {
    return test(k);
}

size_t bitset::size() const {
    return bits_count;
}

bool bitset::empty() const {
    const size_t words = current_block_size();
    for (size_t i = 0; i < words; ++i) if (data[i] != 0) return false;
    return true;
}

void bitset::clear() const {
    if (data) std::memset(data, 0, current_block_size() * sizeof(uint64_t));
}

bitset bitset::union_with(const bitset &other) const {
    bitset result(std::max(bits_count, other.bits_count));

    const size_t min_w = std::min(current_block_size(), other.current_block_size());
    const size_t max_w = result.current_block_size();

    for (size_t i = 0; i < min_w; ++i)
        result.data[i] = data[i] | other.data[i];

    const uint64_t *longer = current_block_size() == max_w ? data : other.data;
    for (size_t i = min_w; i < max_w; ++i)
        result.data[i] = longer[i];

    return result;
}

bitset bitset::intersection(const bitset &other) const {
    const size_t min_b = std::min(current_block_size(), other.current_block_size());
    assert(min_b > 0);

    bitset result(min_b * 64);
    for (size_t i = 0; i < min_b; ++i)
        result.data[i] = data[i] & other.data[i];

    return result;
}

bool bitset::is_subset(const bitset &other) const {
    const size_t curr_w = current_block_size();
    const size_t other_w = other.current_block_size();

    for (size_t i = 0; i < curr_w; ++i) {
        uint64_t curr_word = data[i];
        uint64_t other_word = (i < other_w) ? other.data[i] : 0;

        if ((curr_word & other_word) != curr_word)
            return false;
    }

    return true;
}
