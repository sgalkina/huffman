#pragma once
#include <climits>
#include <bitset>

class Buffer {
  public:
    Buffer();
    bool is_full() const;
    bool is_empty() const;
    void clear();
    void push(bool bit);
    bool pull();
    size_t push_position() const;
    size_t pull_position() const;
    char to_char() const;
  private:
    std::bitset<CHAR_BIT> buffer_;
    size_t push_idx_;
    size_t pull_idx_;
};
