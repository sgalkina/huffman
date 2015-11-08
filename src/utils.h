#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <climits>
#include <bitset>

bool is_empty(std::ifstream & in);

uint8_t read_to_uint(std::ifstream & in_);

void print_vector(std::vector<int> vector);

std::vector<int> convert_to_vector(long s, size_t number);

long vector_to_long(std::vector<int> vector);
