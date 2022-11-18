#ifndef FPA_RANGE_H
#define FPA_RANGE_H

#include <cstdint>
#include <functional>

namespace fpa {

template <typename T> class RangeIterator;
template <typename T> class Range;

template <typename TROW, typename TCOL> class Range2DIterator;
template <typename TROW, typename TCOL> class Range2D;

template <typename T> class RRangeIterator;
template <typename T> class RRange;

template <typename TROW, typename TCOL> class RRange2DIterator;
template <typename TROW, typename TCOL> class RRange2D;

template <typename T> class RangeIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::size_t;
  using value_type = T;
  using reference = T &;
  using pointer = T *;

  RangeIterator(value_type val) : val_(val) {}

  RangeIterator &operator=(const RangeIterator &it) {
    val_ = it.val_;
    return *this;
  }
  bool operator==(const RangeIterator &it) const { return val_ == it.val_; }
  bool operator!=(const RangeIterator &it) const { return val_ != it.val_; }

  RangeIterator &operator++() {
    ++val_;
    return *this;
  }

  RangeIterator operator++(int) {
    RangeIterator tmp = *this;
    ++*this;
    return tmp;
  }

  value_type operator*() const { return val_; }

private:
  value_type val_;
};

template <typename T> class Range {
public:
  using iterator = RangeIterator<T>;
  using value_type = T;

public:
  Range()
      : begin_(static_cast<value_type>(0)), end_(static_cast<value_type>(0)) {}
  Range(value_type begin, value_type end) : begin_(begin), end_(end) {}

  inline iterator begin() const { return empty() ? end() : iterator(begin_); }
  inline typename iterator::value_type beginv() const { return *begin(); }

  inline iterator end() const { return iterator(end_); }
  inline typename iterator::value_type endv() const { return *end(); }

  inline typename iterator::difference_type size() const {
    return empty() ? 0 : end_ - begin_;
  }
  inline bool empty() const { return begin_ >= end_; }

  inline Range &operator==(const Range &rhs) {
    begin_ = rhs.begin_;
    end_ = rhs.end_;
    return *this;
  }
  inline bool operator==(const Range &rhs) const {
    return begin_ == rhs.begin_ && end_ == rhs.end_;
  }

  inline bool operator!=(const Range &rhs) const { return !(*this == rhs); }

private:
  value_type begin_;
  value_type end_;
};

template <typename TROW, typename TCOL = TROW> struct Range2DValue {
  TCOL row;
  TCOL col;
};

template <typename TROW, typename TCOL = TROW> class Range2DIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::size_t;
  using row_type = Range<TROW>;
  using col_type = Range<TCOL>;
  using value_type = Range2DValue<TROW, TCOL>;
  using reference = Range2DValue<TROW, TCOL> &;
  using pointer = Range2DValue<TROW, TCOL> *;

  Range2DIterator(row_type row, col_type col,
                  typename row_type::iterator row_iter,
                  typename col_type::iterator col_iter)
      : row_(row), col_(col), row_iter_(row_iter), col_iter_(col_iter) {}

  bool operator==(const Range2DIterator &it) const { return !(*this != it); }

  bool operator!=(const Range2DIterator &it) const {
    return row_ != it.row_ || col_ != it.col_ || row_iter_ != it.row_iter_ ||
           col_iter_ != it.col_iter_;
  }

  Range2DIterator &operator++() {
    ++col_iter_;
    // next row
    if (col_iter_ == col_.end()) {
      // reset col index
      col_iter_ = col_.begin();
      // increment row index if not end of range2d
      ++row_iter_;
    }

    return *this;
  }

  Range2DIterator operator++(int) {
    Range2DIterator tmp = *this;
    ++*this;
    return tmp;
  }

  value_type operator*() const { return {*row_iter_, *col_iter_}; }

private:
  row_type row_;
  col_type col_;
  typename row_type::iterator row_iter_;
  typename col_type::iterator col_iter_;
};

template <typename TROW, typename TCOL = TROW> class Range2D {
public:
  using iterator = Range2DIterator<TROW, TCOL>;
  using row_type = Range<TROW>;
  using col_type = Range<TCOL>;

public:
  Range2D() {}
  Range2D(row_type row, col_type col) : row_(row), col_(col) {}
  Range2D(TROW begin_row, TROW end_row, TCOL begin_col, TCOL end_col)
      : row_(begin_row, end_row), col_(begin_col, end_col) {}

  inline iterator begin() const {
    return empty() ? end() : iterator(row_, col_, row_.begin(), col_.begin());
  }
  inline typename iterator::value_type beginv() const { return *begin(); }

  inline iterator end() const {
    return iterator(row_, col_, row_.end(), col_.begin());
  }
  inline typename iterator::value_type endv() const { return *end(); }

  inline typename row_type::iterator::difference_type rows() const {
    return row_.size();
  };
  inline typename col_type::iterator::difference_type cols() const {
    return col_.size();
  };
  inline typename iterator::difference_type size() const {
    return row_.size() * col_.size();
  }
  inline const row_type &row() const { return row_; };
  inline const col_type &col() const { return col_; };
  inline bool empty() const { return row_.empty() || col_.empty(); }

  inline bool operator==(const Range2D &rhs) const { return !(*this != rhs); }

  inline bool operator!=(const Range2D &rhs) const {
    return row_ != rhs.row_ || col_ != rhs.col_;
  }

  inline typename iterator::difference_type unit() { return col_.size(); }

private:
  row_type row_;
  col_type col_;
};

template <typename T> class RRangeIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::size_t;
  using value_type = Range<T>;
  using reference = Range<T> &;
  using pointer = Range<T> *;
  using number_type = T;

  // 构造函数
  RRangeIterator(value_type val, value_type cap, number_type offset)
      : val_(val), cap_(cap), offset_(offset) {}

  RRangeIterator<T> &operator=(const RRangeIterator &it) {
    val_ = it.val_;
    return *this;
  }
  bool operator==(const RRangeIterator &it) const { return !(*this != it); }
  bool operator!=(const RRangeIterator &it) const { return val_ != it.val_; }

  RRangeIterator &operator++() {
    auto begin = val_.beginv();
    auto end = val_.endv();

    auto shrinked_size = std::min(offset_, cap_.endv() - val_.endv());
    if (shrinked_size == 0) {
      // this is the last, the next is RRange::end()
      // NOTE: this is the same as container::end()
      val_ = Range<T>(cap_.endv(), cap_.endv());
    } else {
      //  the next is normal
      // if the next is not a whole Range, shrink to fit size.
      val_ = Range<T>(val_.beginv() + offset_, val_.endv() + shrinked_size);
    }
    return *this;
  }

  RRangeIterator operator++(int) {
    RRangeIterator tmp = *this;
    ++*this;
    return tmp;
  }

  value_type operator*() const { return val_; }

private:
  value_type val_;
  value_type cap_;
  number_type offset_;
};

// RRange一个范围迭代器
// 任意一个iterator的代表一个范围, 包含了连续的unit_个值
// 最后一个有效的iterator(end()前面的那个)自适应范围
// 任意相邻的两个iterator之间重叠overlap_个值
// 举例
// 假设overlap_=3, unit_=8, 当前RangeR: [-10, 15), 则:
// begin iter: [-10, -2), unit_: 8
// next1 iter: [-5, 3), unit_: 8, 与begin iter重叠了3个值: [-5, -4, -3]
// next2 iter: [0, 8), unit_: 8, 与next1 iter重叠了3个值: [0, 1, 2]
// next3 iter: [5, 13), unit_: 8, 与next2 iter重叠了3个值: [5, 6, 7]
// next4 iter: [10, 15), unit_: 5, 与next3 iter重叠了3个值: [10, 11, 12]
// 且next4 iter不是一个完整的unit_范围, 被缩减至适应范围了
// end iter: [15, 15), unit_: 0, 不含有效范围
template <typename T> class RRange {
public:
  using iterator = RRangeIterator<T>;
  using value_type = T;

public:
  RRange()
      : begin_(static_cast<value_type>(0)), end_(static_cast<value_type>(0)),
        unit_(static_cast<value_type>(0)),
        overlap_(static_cast<value_type>(0)) {}
  RRange(value_type begin, value_type end, value_type unit, value_type overlap)
      : begin_(begin), end_(end), unit_(unit), overlap_(overlap) {}

  inline iterator begin() const {
    return empty()
               ? end()
               : iterator(typename iterator::value_type(begin_, begin_ + unit_),
                          typename iterator::value_type(begin_, end_),
                          unit_ - overlap_);
  }
  inline typename iterator::value_type beginv() const { return *begin(); }

  inline iterator end() const {
    return iterator(typename iterator::value_type(end_, end_),
                    typename iterator::value_type(end_, end_),
                    unit_ - overlap_);
  }
  inline typename iterator::value_type endv() const { return *end(); }

  inline typename iterator::difference_type size() const {
    return std::distance(begin(), end());
  }
  inline bool empty() const { return begin_ >= end_; }

private:
  value_type begin_;
  value_type end_;
  value_type unit_;
  value_type overlap_;
};

template <typename TROW, typename TCOL = TROW> class RRange2DIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::size_t;
  using value_type = Range2D<TROW, TCOL>;
  using reference = Range2D<TROW, TCOL> &;
  using pointer = Range2D<TROW, TCOL> *;
  using container_type = RRange2D<TROW, TCOL>;
  using row_type = typename container_type::row_type;
  using col_type = typename container_type::col_type;

  RRange2DIterator(row_type row, col_type col,
                   typename row_type::iterator row_iter,
                   typename col_type::iterator col_iter)
      : row_(row), col_(col), row_iter_(row_iter), col_iter_(col_iter) {}

  bool operator==(const RRange2DIterator &it) const { return !(*this != it); }

  bool operator!=(const RRange2DIterator &it) const {
    return row_iter_ != it.row_iter_ || col_iter_ != it.col_iter_;
  }

  RRange2DIterator &operator++() {
    ++col_iter_;
    if (col_iter_ == col_.end()) {
      col_iter_ = col_.begin();
      ++row_iter_;
    }
    return *this;
  }

  RRange2DIterator operator++(int) {
    RRange2DIterator tmp = *this;
    ++*this;
    return tmp;
  }

  value_type operator*() const {
    return Range2D<TROW, TCOL>(*row_iter_, *col_iter_);
  }

private:
  row_type row_;
  col_type col_;
  typename row_type::iterator row_iter_;
  typename col_type::iterator col_iter_;
}; // namespace fpa

template <typename TROW, typename TCOL = TROW> class RRange2D {
public:
  using iterator = RRange2DIterator<TROW, TCOL>;
  using row_type = RRange<TROW>;
  using col_type = RRange<TCOL>;

public:
  RRange2D() {}
  RRange2D(const row_type &row, const col_type &col) : row_(row), col_(col) {}
  RRange2D(TROW begin_row, TROW end_row, TROW unit_row, TROW overlap_row,
           TCOL begin_col, TCOL end_col, TCOL unit_col, TCOL overlap_col)
      : row_(begin_row, end_row, unit_row, overlap_row),
        col_(begin_col, end_col, unit_col, overlap_col) {}

  inline iterator begin() const {
    return empty() ? end() : iterator(row_, col_, row_.begin(), col_.begin());
  }
  inline typename iterator::value_type beginv() const { return *begin(); }

  inline iterator end() const {
    return iterator(row_, col_, row_.end(), col_.begin());
  }
  inline typename iterator::value_type endv() const { return *end(); }

  inline typename row_type::iterator::difference_type rows() const {
    return row_.size();
  };
  inline typename col_type::iterator::difference_type cols() const {
    return col_.size();
  };
  inline typename iterator::difference_type size() const {
    return row_.size() * col_.size();
  }
  inline bool empty() const { return row_.empty() || col_.empty(); }

  inline const row_type &row() const { return row_; };
  inline const col_type &col() const { return col_; };

private:
  row_type row_;
  col_type col_;
};
} // namespace fpa

#endif // FPA_RANGE_H