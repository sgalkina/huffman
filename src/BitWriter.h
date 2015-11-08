#pragma once
#include <fstream>
#include <vector>
#include "utils.h"
#include "Buffer.h"

class BitWriter {
  public:
    BitWriter(std::ofstream & out);
    void write_vector(const std::vector<int> &prefix);
    void write_one_bit(bool bit);
    void write_end();
    size_t output_size() const;
  private:
    void flush(); 
    std::ofstream & out_;
    size_t output_size_counter_;
    Buffer buffer_;
};
