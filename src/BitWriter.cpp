#include "BitWriter.h"

BitWriter::BitWriter(std::ofstream & out)
  : out_(out)
  , output_size_counter_(0)
  , buffer_()
  {}


void BitWriter::flush() {
  char a = buffer_.to_char();
  out_.write(&a, 1);
  buffer_.clear();
  ++output_size_counter_;
}

void BitWriter::write_vector(const std::vector<int> &vector) {
  for (auto& v : vector) {
    write_one_bit(v);
  }
}

void BitWriter::write_one_bit(bool bit) {
  buffer_.push(bit);
  if (buffer_.is_full()) {
      flush();
  }
}

void BitWriter::write_end() {
  uint8_t last_bits_count = buffer_.is_empty() ? CHAR_BIT : buffer_.push_position();
  while(!buffer_.is_empty()) {
    write_one_bit(0);
  }
  out_.write((char *) &last_bits_count, 1);
}

size_t BitWriter::output_size() const {
  return output_size_counter_;
}
