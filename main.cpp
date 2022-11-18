#include <iostream>

#include <iomanip>
#include <iterator>
#include <list>
#include <sstream>
#include <vector>

#include "rrange.h"

template <typename RANGE, typename SEPRATOR>
std::string join(const RANGE &range, SEPRATOR sep) {
  if (range.empty()) {
    return {};
  }
  auto itr = range.begin();
  std::ostringstream sout;
  sout << *itr;
  while (++itr != range.end()) {
    sout << sep;
    sout << *itr;
  }
  return sout.str();
}

int main(int, char **) {

  auto range_to_string = [&](fpa::Range<int> &r) { return join(r, " "); };

  auto rrange_to_string = [&](fpa::RRange<int> &rr) {
    std::vector<std::string> strings;
    strings.reserve(rr.size());

    for (auto r : rr) {
      strings.push_back(range_to_string(r));
    }
    return strings;
  };

  auto range2d_to_string = [&](fpa::Range2D<int> &r2d) {
    std::vector<std::string> strings;
    strings.reserve(r2d.rows());

    int col = 0, cols = r2d.cols();
    std::string str;
    for (auto v : r2d) {
      str.append(std::to_string(v.row) + "." + std::to_string(v.col) + " ");
      if (++col == cols) {
        col = 0;
        strings.push_back(std::move(str));
      }
    }
    if (col != 0) {
      strings.push_back(std::move(str));
    }
    return strings;
  };

  auto rrange2d_to_string = [&](fpa::RRange2D<int> &rr2d) {
    int col = 0, cols = rr2d.cols();
    int row_pos = 0;
    std::vector<std::string> strings;
    std::string str;
    for (auto r2d : rr2d) {
      auto substrings = range2d_to_string(r2d);
      if (col == 0) {
        strings.resize(strings.size() + substrings.size() + 1);
      }
      for (size_t i = 0; i < substrings.size(); i++) {
        strings[row_pos + i].append(std::move(substrings[i]));
        strings[row_pos + i].append("  ");
      }
      if (++col == cols) {
        col = 0;
        row_pos = strings.size();
      }
    }
    return strings;
  };

  { // Range2D
    int col = 10;
    int row = 10;
    fpa::Range2D<int> r2d(0, 0 + row, 0, 0 + col);
    std::cout << "rows: " << r2d.rows() << ", cols: " << r2d.cols()
              << ", size: " << r2d.size() << ", is empty: " << std::boolalpha
              << r2d.empty() << std::endl;
    std::cout << join(range2d_to_string(r2d), "\n") << std::endl;
  }

  std::cout << "---------------------------------------------------------------"
            << std::endl;

  { // RRange
    int delta = 4;
    int overlap = 1;
    fpa::RRange<int> rr(1, 10, delta, overlap);

    std::cout << "size: " << rr.size() << ", is empty: " << std::boolalpha
              << rr.empty() << std::endl;
    std::cout << join(rrange_to_string(rr), "\n") << std::endl;
  }

  std::cout << "---------------------------------------------------------------"
            << std::endl;

  { // RRange2D
    int col = 20;
    int row = 20;
    int delta_row = 6;
    int delta_col = 6;
    int overlap_row = 2;
    int overlap_col = 2;

    fpa::RRange2D<int> rr2d(10, 10 + row, delta_row, overlap_row, 50, 50 + col,
                            delta_col, overlap_col);

    std::cout << "rows: " << rr2d.rows() << ", cols: " << rr2d.cols()
              << ", size: " << rr2d.size() << ", is empty: " << std::boolalpha
              << rr2d.empty() << std::endl;
    std::cout << join(rrange2d_to_string(rr2d), "\n") << std::endl;
  }

  return 0;
}
