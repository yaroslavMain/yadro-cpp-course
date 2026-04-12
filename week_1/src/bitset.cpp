#include "bitset.h"


size_t bitset::words_for_bits(const size_t bits) noexcept {
    return (bits + bits_per_word - 1) / bits_per_word;
}

size_t bitset::block_index(const size_t pos) noexcept {
    return pos / bits_per_word;
}

size_t bitset::bit_mask(const size_t pos) noexcept {
    return 1ULL << (pos % bits_per_word);
}


size_t bitset::word_capacity_count() const noexcept {
    return words_for_bits(capacity_);
}


void bitset::resize(const size_t new_size) {
    if (new_size <= capacity_) return;

    const size_t old_words = word_capacity_count();
    const size_t new_words = words_for_bits(new_size);

    auto *new_data = new(std::nothrow) uint64_t[new_words]();
    if (!new_data) return;;

    if (data_ && old_words > 0) {
        std::memcpy(new_data, data_, old_words * bytes_per_word);
    }

    delete[] data_;
    data_ = new_data;
    capacity_ = new_words * bits_per_word;
}

bitset::bitset() = default;

bitset::bitset(const size_t initial_capacity) {
    if (initial_capacity > 0)
        resize(initial_capacity);
}

bitset::~bitset() {
    delete[] data_;
}

bitset::bitset(const bitset &other) : capacity_(other.capacity_) {
    const size_t other_words = words_for_bits(other.capacity_);
    if (other_words > 0) {
        data_ = new uint64_t[other_words]();
        std::memcpy(data_, other.data_, other_words * bytes_per_word);
    } else {
        data_ = nullptr;
    }
}

bitset &bitset::operator=(const bitset &other) {
    if (this == &other) return *this;

    delete[] data_;

    data_ = nullptr;
    capacity_ = other.capacity_;

    const size_t other_words = words_for_bits(other.capacity_);
    if (other_words > 0) {
        data_ = new uint64_t[other_words]();
        std::memcpy(data_, other.data_, other_words * bytes_per_word);
    }

    return *this;
}

bitset::bitset(bitset &&other) noexcept : data_(other.data_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.capacity_ = 0;
}

bitset &bitset::operator=(bitset &&other) noexcept {
    if (this == &other) return *this;

    delete[] data_;

    data_ = other.data_;
    capacity_ = other.capacity_;

    other.data_ = nullptr;
    other.capacity_ = 0;

    return *this;
}

void bitset::set(const size_t k, const bool b) {
    const size_t block = block_index(k);
    const size_t mask = bit_mask(k);

    if (b) {
        if (k >= capacity_)
            resize(std::max(k + 1, capacity_ * 2));

        if (!data_) return;
        data_[block] |= mask;
    } else if (k < capacity_ && data_) {
        data_[block] &= ~mask;
    }
}

bool bitset::test(const size_t k) const {
    if (!data_ && k >= capacity_) return false;
    return data_[block_index(k)] & bit_mask(k);
}

bool bitset::operator[](const size_t k) const {
    return test(k);
}

size_t bitset::size() const {
    return capacity_;
}

bool bitset::empty() const {
    if (!data_) return true;

    const size_t words = word_capacity_count();
    for (size_t i = 0; i < words; ++i)
        if (data_[i] != 0)
            return false;

    return true;
}

void bitset::clear() noexcept {
    if (data_)
        std::memset(data_, 0, word_capacity_count() * bytes_per_word);
}

bitset bitset::union_with(const bitset &other) const {
    bitset result(std::max(capacity_, other.capacity_));

    const size_t min_w = std::min(word_capacity_count(), other.word_capacity_count());
    const size_t max_w = result.word_capacity_count();

    for (size_t i = 0; i < min_w; ++i)
        result.data_[i] = data_[i] | other.data_[i];

    const uint64_t *longer = (word_capacity_count() >= other.word_capacity_count()) ? data_ : other.data_;
    for (size_t i = min_w; i < max_w; ++i)
        result.data_[i] = longer[i];


    return result;
}

bitset bitset::intersection(const bitset &other) const {
    const size_t min_word_count = std::min(word_capacity_count(), other.word_capacity_count());

    bitset result(min_word_count * bits_per_word);
    for (size_t i = 0; i < min_word_count; ++i)
        result.data_[i] = data_[i] & other.data_[i];

    return result;
}

bool bitset::is_subset(const bitset &other) const {
    const size_t curr_word_count = word_capacity_count();
    const size_t other_word_count = other.word_capacity_count();

    for (size_t i = 0; i < curr_word_count; ++i) {
        const uint64_t curr_word = data_[i];

        if (const uint64_t other_word = (i < other_word_count) ? other.data_[i] : 0;
            (curr_word & other_word) != curr_word)
            return false;
    }

    return true;
}
