#include "Buffer.h"

Buffer::Buffer()
  : buffer_()
  , push_idx_(0)
  , pull_idx_(0)
  {}

void Buffer::push(bool bit) {
  buffer_.set(push_idx_++, bit);
}

bool Buffer::pull() {
  bool result = buffer_[pull_idx_++];
  if (pull_idx_ == CHAR_BIT) pull_idx_ = 0;
  return result;
}

bool Buffer::is_full() const {
  return push_position() == CHAR_BIT;
}

bool Buffer::is_empty() const {
  return push_position() == 0;
}

void Buffer::clear() {
  buffer_.reset();
  push_idx_ = 0;
}

size_t Buffer::push_position() const {
  return push_idx_;
}

size_t Buffer::pull_position() const {
  return pull_idx_;
}

char Buffer::to_char() const {
  return buffer_.to_ulong();
}
