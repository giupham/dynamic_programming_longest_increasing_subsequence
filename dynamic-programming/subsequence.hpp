
///////////////////////////////////////////////////////////////////////////////
// subsequence.hpp
//
// Dynamic programming algorithm for solving the longest increasing
// subsequence problem.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <sstream>
#include <vector>

using sequence = std::vector<int>;

// Convert a sequence into a human-readable string useful for pretty-printing
// or debugging.
std::string sequence_to_string(const sequence& seq) {
  std::stringstream ss;
  ss << "[";
  bool first = true;
  for (auto& x : seq) {
    if (!first) {
      ss << ", ";
    }
    ss << x;
    first = false;
  }
  ss << "]";
  return ss.str();
}

// Generate a pseudorandom sequence of the given size, using the given
// seed, where all elements are in the range [0, max_element]. max_element
// must be non-negative.
sequence random_sequence(size_t size, unsigned seed, int max_element) {

    assert(max_element >= 0);

    sequence result;

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(0, max_element);

    for (size_t i = 0; i < size; ++i) {
        result.push_back(dist(gen));
    }

    return result;
}

bool is_increasing(const sequence& A) {
  for (size_t i = 1; i < A.size(); ++i) {
    if (!(A.at(i - 1) < A.at(i))) return false;
  }
  return true;
}

sequence longest_increasing_end_to_beginning(const sequence& A) {

  const size_t n = A.size();

  // populate the array H with 0 values
  std::vector<size_t> H(n, 0);

  // calculate the values of array H
  // note that i has to be declared signed, to avoid an infinite loop, since
  // the loop condition is i >= 0
  for (signed int i = n-2;  i>= 0; i--) {
    for (size_t j = i+1; j < n ; j++) {
        if (A[i] < A[j]) {
          if (H[i] <= H[j]) H[i] = H[j] + 1;
        }
    }
  }

  // calculate in max the length of the longest subsequence
    // by adding 1 to the maximum value in H
  auto max = *std::max_element(H.begin(), H.end()) + 1;
  // allocate space for the subsequence R
  std::vector<int> R(max, 0);

  // add elements to R by whose H's values are in decreasing order,
    // starting with max-1
  // store in index the H values sought

    size_t index = max-1, j = 0;
    for (size_t i = 0; i < n; ++i) {
      if (H[i] == index) {
          R[j++] = A[i];
          index--;
      }
    }
  return sequence(R.begin(), R.begin() + max);
}
