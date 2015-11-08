#pragma once
#include <queue>
#include <deque>
#include <utility>
#include <vector>
#include <array>
#include <unordered_map>
#include <climits>
#include "utils.h"

class Node;

class Tree {
      public:
        Tree(const std::unordered_map <uint8_t, uint64_t> &frequencies);
        Tree(std::vector<int> &tree_serialized);
        ~Tree();
        void fill_prefix_table();
        void print_prefix_table() const;
        std::vector<int> const &prefix_by_letter(uint8_t letter) const;
        void move_current_position(bool bit);
        bool is_leaf() const;
        uint8_t decode();
        std::vector<int> serialize() const;
        Node * deserialize(std::deque<bool> & key);
      private:
        Node* root_;
        Node* current_position_;
        std::unordered_map<uint8_t, std::vector<int>> prefix_table_;
};
