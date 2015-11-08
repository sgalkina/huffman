#include "Tree.h"
#include "BitReader.h"
#include "BitWriter.h"
#include "utils.h"
#include <unordered_map>

using Frequencies = std::unordered_map <uint8_t, uint64_t>;

Frequencies count_frequencies(std::ifstream & f) {
    Frequencies frequencies;
    char memblock[1];
    while (f.read(memblock, 1)) {
        frequencies[*memblock]++;
    }
    return frequencies;
}

void print_statistics(size_t size_input, size_t size_output, size_t size_tree) {
  std::cout << size_input << std::endl;
  std::cout << size_output << std::endl;
  std::cout << size_tree << std::endl;
}

void encode(std::ifstream & in, std::ofstream & out, const Frequencies &frequencies) {
  if (is_empty(in)) {
    print_statistics(0, 0, 0);
    return;
  }
  Tree tree = Tree(frequencies);
  BitWriter bit_writer = BitWriter(out);
  bit_writer.write_vector(tree.serialize());
  size_t tree_size = bit_writer.output_size();
  size_t input_size = 0;
  while (!in.eof()) {
    uint8_t l = read_to_uint(in);
    if (!in.eof()) {
      bit_writer.write_vector(tree.prefix_by_letter(l));
      ++input_size;
    }
  }
  bit_writer.write_end();
  print_statistics(input_size, bit_writer.output_size() - tree_size, tree_size);
}

void decode(std::ifstream & in, std::ofstream & out) {
  if (is_empty(in)) {
    print_statistics(0, 0, 0);
    return;
  }
  BitReader bit_reader(in);
  size_t key_size = vector_to_long(bit_reader.extract_n_bits(16));
  std::vector<int> tree_serialized = bit_reader.extract_n_bits(key_size);
  Tree tree = Tree(tree_serialized);
  size_t output_size = 0;
  while (!bit_reader.end()) {
    tree.move_current_position(bit_reader.extract_one_bit());
    if (tree.is_leaf()) {
      out << tree.decode();
      ++output_size;
    }
  }
  size_t tree_size = 2 + key_size/8;
  print_statistics(bit_reader.input_size() - tree_size, output_size, tree_size);
}

int main(int argc, char* argv[]) {
    bool to_encode;
    std::string in_filename;
    std::string out_filename;
    for (unsigned i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-c") {
            to_encode = true;
        } else if (arg == "-u") {
            to_encode = false;
        } else if (arg == "-f" || arg == "--file") {
            in_filename = argv[++i];
        } else if (arg == "-o" || arg == "--output") {
            out_filename = argv[++i];
        }
    }
    if (to_encode) {
        std::ifstream in(in_filename.c_str(), std::ofstream::binary);
        std::ofstream out(out_filename.c_str(), std::ofstream::binary);
        auto frequencies = count_frequencies(in);
        in.clear();
        in.seekg(0, std::ios::beg);
        encode(in, out, frequencies);
    } else {
        std::ifstream in(in_filename.c_str(), std::ifstream::binary);
        std::ofstream out(out_filename.c_str(), std::ofstream::binary);
        decode(in, out);
    }
    return 0;
}
