#include <iostream>

#include <iomanip>
#include <iterator>
#include <list>
#include <sstream>
#include <vector>

#include "range.h"

int main(int, char **) {
  auto range_to_string = [&](fpa::Range<int> &r) {
    for (auto v : r) {
      std::cout << v << " ";
    }
    std::cout << "\n";
  };

  auto rrange_to_string = [&](fpa::RRange<int> &rr) {
    for (auto &r : rr) {
      range_to_string(r);
    }
    std::cout << "\n";
    std::cout << "\n";
  };

  auto range2d_to_string = [&](fpa::Range2D<int> &rr, int width) {
    int i = 0;
    for (auto &v : rr) {
      std::cout << v.row << "." << v.col << " ";
      if (++i == width) {
        i = 0;
        std::cout << "\n";
      }
    }
    if (i != 0) {
      std::cout << "\n";
    }
    std::cout << "\n";
  };

  auto rrange2d_to_string = [&](fpa::RRange2D<int> &rr2d) {
    int i = 0;
    for (auto &r2d : rr2d) {
      range2d_to_string(r2d, r2d.col_.size());
    }
    std::cout << "\n";
  };

  { // Range2D
    int col = 20;
    int row = 30;
    fpa::Range2D<int> r2d(11, 11 + row, 55, 55 + col);
    std::cout << "size: " << r2d.size() << ", is empty: " << std::boolalpha
              << r2d.empty() << std::endl;
    range2d_to_string(r2d, col);
  }

  { // RRange
    int delta = 8;
    int overlap = 3;
    fpa::RRange<int> rr(14, 150, delta, overlap);

    std::cout << "size: " << rr.size() << ", is empty: " << std::boolalpha
              << rr.empty() << std::endl;
    rrange_to_string(rr);
  }

  { // RRange2D
    int col = 32;
    int row = 42;
    int delta_row = 10;
    int delta_col = 10;
    int overlap_row = 1;
    int overlap_col = 1;

    fpa::RRange2D<int> rr2d(100, 100 + row, delta_row, overlap_row, 200,
                            200 + col, delta_col, overlap_col);

    std::cout << "size: " << rr2d.size() << ", is empty: " << std::boolalpha
              << rr2d.empty() << std::endl;
    rrange2d_to_string(rr2d);
  }

  return 0;
}