#include "utils.h"

bool is_empty(std::ifstream & in) {
  return in.peek() == std::ifstream::traits_type::eof();
}

uint8_t read_to_uint(std::ifstream & in_) {
  uint8_t memblock[1];
  in_.read((char *) memblock, 1); 
  return *memblock;
}

void print_vector(std::vector<int> vector) {
  for ( auto &i : vector ) {
      std::cout << i;
  }
  std::cout << std::endl;
}

long vector_to_long(std::vector<int> vector) {
  std::stringstream stream;
  std::copy(vector.begin(), vector.end(), std::ostream_iterator<int>(stream, ""));
  if (vector.size() == CHAR_BIT) return std::bitset<8>(stream.str()).to_ulong();
  return std::bitset<16>(stream.str()).to_ulong();
}

std::vector<int> convert_to_vector(long s, size_t number) {
  std::vector<int> result;
  for (size_t i = 0; i < number; ++i) {
    result.push_back((s >> i) & 1);
  }
  std::reverse(result.begin(), result.end());
  return result;
}
