#pragma once
#include <fstream>
#include <vector>
#include "utils.h"
#include "Buffer.h"

class BitReader {
  public:
    BitReader(std::ifstream & in);
    bool extract_one_bit();
    std::vector<int> extract_n_bits(size_t n);
    bool end();
    size_t input_size() const;
  private:
    void read_next_byte_();
    std::ifstream & in_;
    size_t last_byte_;
    size_t n_last_bits_;
    size_t input_size_counter_;
    Buffer buffer_;
};
