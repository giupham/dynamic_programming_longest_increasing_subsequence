///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
  vector<disk_color> _colors;

public:

  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

      assert(light_count > 0);

      for (size_t i = 0; i < _colors.size(); i += 2) {
        _colors[i] = DISK_DARK;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  string to_string() const {
    stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is dark, the second disk at index 1
  // is light, and so on for the entire row of disks.
  bool is_alternating() const {
    if (_colors.empty() || total_count()<2) {
      return false;
    }
    else {
      for (size_t j=0; j<total_count(); j=j+2) {
        if (_colors[j] == DISK_LIGHT && _colors[j+1] == DISK_DARK) {
          return false;
        }
      }
      return true;
    }
  }


  bool is_sorted() const {
    if (_colors.empty() || total_count() < 2) {
      return true;
    }
    else {
      size_t i=0;
      for (i=0; i<total_count()/2; i++) {
        if (_colors[i]!=DISK_LIGHT) {
          return false;
        }
      }
      for (size_t j=i; j<total_count(); j++) {
        if (_colors[j]!=DISK_DARK) {
          return false;
        }
      }
    }
    return true;
  }
};


// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {
  unsigned swaps = 0;
  disk_state sorted = before;
  unsigned n = sorted.total_count()/2;
  for (size_t i=0; i<n; i++) {
    for (size_t j=i; j<2*n-i-1; j++) {
      if (sorted.get(j)==DISK_DARK && sorted.get(j+1)==DISK_LIGHT) {
        sorted.swap(j);
        swaps++;
      }
    }
  }
  return sorted_disks(sorted, swaps);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  unsigned swaps = 0;
  disk_state sorted = before;
  unsigned n = sorted.total_count()/2;
  size_t ceiling = ceil(n/2);
  for (size_t i=0; i<ceiling+1; i++){
    for (size_t j=0; j<2*n-i-1; j++){
      if (sorted.get(j)==DISK_DARK && sorted.get(j+1)==DISK_LIGHT) {
        sorted.swap(j);
        swaps++;
      }
    }
    for (size_t k=2*n-i-1; k>0; k--){
      if (sorted.get(k)==DISK_LIGHT && sorted.get(k-1)==DISK_DARK) {
        sorted.swap(k-1);
        swaps++;
      }
    }
  }
  return sorted_disks(sorted, swaps);
}
