#include <vector>
#include <iostream>
#include <map>
#include <bitset>
#include <exception>
#include <deque>
#include "Tree.h"

class Node {
      public:
        Node(uint8_t letter, int frequency=0)
          : l_(letter)
          , f_(frequency)
          , children_{{nullptr, nullptr}}
        {};

        Node(Node * l, Node * r)
          : l_()
          , f_(l->frequency() + r->frequency())
          , children_{{l, r}}
        {};

        ~Node() {
          delete children_[0];
          delete children_[1];
        };

        std::vector<int> serialize() const {
          if (is_leaf()) {
            std::vector<int> result = convert_to_vector(l_, CHAR_BIT);
            result.insert(result.begin(), is_leaf());
            return result;
          }
          std::vector<int> left = children_[0]->serialize();
          left.insert(left.begin(), is_leaf());
          std::vector<int> right = children_[1]->serialize();
          left.insert(left.end(), right.begin(), right.end());
          return left;
        };

        void bitcode(std::unordered_map<uint8_t, std::vector<int>> & bitcodes_table, const std::vector<int> &current) const {
          if (is_leaf()) {
            if (current.empty()) {
              std::vector<int> v{0};
              bitcodes_table[l_] = v;
            } else {
              bitcodes_table[l_] = current;
            }
            return;
          }
          for (size_t i = 0; i < 2; ++i) {
            std::vector<int> branch = current;
            branch.push_back(i);
            children_[i]->bitcode(bitcodes_table, branch);
          }
        };

        int frequency() const {
          return f_;
        };

        uint8_t letter() const {
          return l_;
        };

        Node* go_to(bool bit) {
          return children_[bit];
        };

        bool is_leaf() const {
          return children_[0] == nullptr && children_[1] == nullptr;
        };
      private:
        uint8_t l_;
        int f_;
        std::array<Node *, 2> children_;
};

auto comp = [](Node *i, Node *j) { return (i->frequency() > j->frequency()); };
using NodesQueue = std::priority_queue<Node*, std::vector<Node*>, decltype(comp)>;

Tree::Tree(const std::unordered_map <uint8_t, uint64_t> &frequencies)
  : root_(nullptr)
  , current_position_(nullptr)
  , prefix_table_()
  {
    NodesQueue nodes_queue(comp);
    for(auto & it : frequencies) {
      nodes_queue.push(new Node(it.first, it.second));
    }

    while (nodes_queue.size() > 1) {
      Node *left = nodes_queue.top();
      nodes_queue.pop();

      Node *right = nodes_queue.top();
      nodes_queue.pop();

      nodes_queue.push(new Node(left, right));
    }
    root_ = nodes_queue.top();
    current_position_ = root_;
    fill_prefix_table();
  }

Tree::Tree(std::vector<int> & tree_serialized)
  : root_(nullptr)
  , current_position_(nullptr)
  , prefix_table_()
  {
    std::deque<bool> key(tree_serialized.begin(), tree_serialized.end());
    root_ = deserialize(key);
    current_position_ = root_;
  }

Node * Tree::deserialize(std::deque<bool> & key) {
  if (!key.empty()) {
    bool front = key.front();
    key.pop_front();
    if (front) {
        std::string letter;
        for (size_t i = 0 ; i < CHAR_BIT; ++i) {
          letter += std::to_string(key.front());
          key.pop_front();
        }
        return new Node(std::bitset<CHAR_BIT>(letter).to_ulong());
    } else {
        Node *left = deserialize(key);
        Node *right = deserialize(key);
        return new Node(left, right);
    }
  }
  return nullptr;
}

Tree::~Tree() {
  delete root_;
}

void Tree::fill_prefix_table() {
  std::vector<int> empty;
  root_->bitcode(prefix_table_, empty);
}

std::vector<int> const &Tree::prefix_by_letter(uint8_t letter) const {
  auto const &it = prefix_table_.find(letter);
  return it->second;
}

std::vector<int> Tree::serialize() const {
  std::vector<int> key = root_->serialize();
  std::vector<int> result = convert_to_vector(key.size(), 16);
  result.insert(result.end(), key.begin(), key.end());
  return result;
}

void Tree::move_current_position(bool bit) {
  current_position_ = is_leaf() ? root_ : current_position_->go_to(bit);
}

bool Tree::is_leaf() const {
  return current_position_->is_leaf();
}

uint8_t Tree::decode() {
  if (!current_position_->is_leaf()) throw "Not leaf";
  uint8_t result = current_position_->letter();
  current_position_ = root_;
  return result;
}

void Tree::print_prefix_table() const {
    for(auto const& it : prefix_table_) {
        std::cout << it.first << " ";
        for ( auto &i : it.second ) {
            std::cout << i;
        }
        std::cout << std::endl;
    }
}
