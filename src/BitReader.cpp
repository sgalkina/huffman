#include "BitReader.h"

namespace {
  size_t last_byte_position(std::ifstream &in) {
    size_t current = in.tellg();
    in.seekg(-1, std::ios::end);
    size_t result = in.tellg();
    in.seekg(current, std::ios::beg);
    return result;
  }

  size_t last_bits_to_read(std::ifstream &in) {
    size_t current = in.tellg();
    in.seekg(-1, std::ios::end);
    char memblock[1];
    in.read(memblock, 1);
    int result = *memblock;
    in.seekg(current, std::ios::beg);
    return result;
  }
}

BitReader::BitReader(std::ifstream & in)
  : in_(in)
  , last_byte_()
  , n_last_bits_()
  , input_size_counter_()
  , buffer_()
  {
    last_byte_ = last_byte_position(in_);
    n_last_bits_ = last_bits_to_read(in_);
  }

bool BitReader::extract_one_bit() {
  if (buffer_.pull_position() == 0) read_next_byte_();
  return buffer_.pull();
}

std::vector<int>  BitReader::extract_n_bits(size_t n) {
  std::vector<int> result;
  for (size_t i = 0; i < n; ++i) {
    result.push_back(extract_one_bit());
  }
  return result;
}

void BitReader::read_next_byte_() {
  uint8_t a = read_to_uint(in_);
  ++input_size_counter_;
  size_t to_read = input_size_counter_ == last_byte_ ? n_last_bits_: CHAR_BIT;
  buffer_.clear();
  for (size_t i = 0; i < to_read; ++i) {
    buffer_.push((a >> i) & 1);
  }
}

bool BitReader::end() {
  return input_size_counter_ == last_byte_ && buffer_.pull_position() == n_last_bits_;
}

size_t BitReader::input_size() const {
  return input_size_counter_;
}
