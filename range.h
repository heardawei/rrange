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

  value_type &operator*() { return val_; }

private:
  value_type val_;
};

template <typename T> class Range {
  friend class RRangeIterator<T>;
  friend class RRange2DIterator<T, T>;

public:
  using iterator = RangeIterator<T>;

public:
  Range() : begin_(static_cast<T>(0)), end_(static_cast<T>(0)) {}
  Range(T begin, T end) : begin_(begin), end_(end) {}

  inline iterator cbegin() const { return empty() ? cend() : iterator(begin_); }
  inline iterator begin() { return empty() ? end() : iterator(begin_); }

  inline iterator cend() const { return iterator(end_); }
  inline iterator end() { return iterator(end_); }

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
  T begin_;
  T end_;
};

template <typename TROW, typename TCOL = TROW> struct Range2DValue {
  TCOL row;
  TCOL col;
};

template <typename TROW, typename TCOL = TROW> class Range2DIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::size_t;
  using value_type = Range2DValue<TROW, TCOL>;
  using reference = Range2DValue<TROW, TCOL> &;
  using pointer = Range2DValue<TROW, TCOL> *;

  Range2DIterator(value_type val, const Range2D<TROW, TCOL> *container)
      : val(val), container_(container) {}

  bool operator==(const Range2DIterator &it) const { return !(*this != it); }

  bool operator!=(const Range2DIterator &it) const {
    return val.col != it.val.col || val.row != it.val.row;
  }

  Range2DIterator &operator++() {
    typename Range<TROW>::iterator row_iter(val.row);
    typename Range<TCOL>::iterator col_iter(val.col);
    ++col_iter;
    // next row
    if (col_iter == container_->col_.cend()) {
      // reset col index
      col_iter = container_->col_.cbegin();
      // increment row index if not end of range2d
      ++row_iter;
    }

    val.col = *col_iter;
    val.row = *row_iter;

    return *this;
  }

  Range2DIterator operator++(int) {
    Range2DIterator tmp = *this;
    ++*this;
    return tmp;
  }

  value_type &operator*() { return val; }

private:
  value_type val;
  const Range2D<TROW, TCOL> *container_;
};

template <typename TROW, typename TCOL = TROW> class Range2D {
  friend class RRange2DIterator<TROW, TCOL>;

public:
  using iterator = Range2DIterator<TROW, TCOL>;

public:
  Range2D() {}
  Range2D(TROW begin_row, TROW end_row, TCOL begin_col, TCOL end_col)
      : row_(begin_row, end_row), col_(begin_col, end_col) {}

  inline iterator cbegin() const {
    return empty() ? cend() : iterator({*row_.begin(), *col_.begin()}, this);
  }
  inline iterator begin() {
    return empty() ? end() : iterator({*row_.begin(), *col_.begin()}, this);
  }

  inline iterator cend() const {
    return iterator({*row_.end(), *col_.begin()}, this);
  }
  inline iterator end() { return iterator({*row_.end(), *col_.begin()}, this); }

  inline typename iterator::difference_type size() const {
    return row_.size() * col_.size();
  }
  inline bool empty() const { return row_.empty() || col_.empty(); }

  inline bool operator==(const Range2D &rhs) const { return !(*this != rhs); }

  inline bool operator!=(const Range2D &rhs) const {
    return row_ != rhs.row_ || col_ != rhs.col_;
  }

private:
  Range<TROW> row_;

public:
  Range<TCOL> col_;

private:
  friend class Range2DIterator<TROW, TCOL>;
};

template <typename T> class RRangeIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::size_t;
  using value_type = Range<T>;
  using reference = Range<T> &;
  using pointer = Range<T> *;

  // 构造函数
  RRangeIterator(T begin, T end, const RRange<T> *container)
      : val_(begin, end), container_(container) {}

  RRangeIterator<T> &operator=(const RRangeIterator &it) {
    val_ = it.val_;
    return *this;
  }
  bool operator==(const RRangeIterator &it) const { return !(*this != it); }
  bool operator!=(const RRangeIterator &it) const { return val_ != it.val_; }

  RRangeIterator &operator++() {
    auto begin = val_.begin_;
    auto end = val_.end_;

    if (end == container_->end_) {
      // this is the last, the next is RRange::end()
      (*this) = container_->cend();
    } else {
      //  the next is normal
      auto offset = (container_->delta_ - container_->overlap_);
      // if the next is not a whole Range, shrink to fit size.
      begin += offset;
      end += std::min(offset, container_->end_ - end);
      val_ = Range<T>(begin, end);
    }
    return *this;
  }

  RRangeIterator operator++(int) {
    RRangeIterator tmp = *this;
    ++*this;
    return tmp;
  }

  value_type &operator*() { return val_; }

private:
  Range<T> val_;
  const RRange<T> *container_;
};

template <typename T> class RRange {
  friend class RRangeIterator<T>;
  friend class RRange2D<T, T>;
  friend class RRange2DIterator<T, T>;

public:
  using iterator = RRangeIterator<T>;

public:
  RRange()
      : begin_(static_cast<T>(0)), end_(static_cast<T>(0)),
        delta_(static_cast<T>(0)) {}
  RRange(T begin, T end, T delta, T overlap)
      : begin_(begin), end_(end), delta_(delta), overlap_(overlap) {}

  inline iterator cbegin() const {
    return empty() ? cend()
                   : iterator(begin_, std::min(begin_ + delta_, end_), this);
  }
  inline iterator begin() {
    return empty() ? end()
                   : iterator(begin_, std::min(begin_ + delta_, end_), this);
  }

  inline iterator cend() const { return iterator(end_, end_, this); }
  inline iterator end() { return iterator(end_, end_, this); }

  inline typename iterator::difference_type size() const {
    return empty() ? 0 : std::distance(cbegin(), cend());
  }
  inline bool empty() const { return begin_ >= end_; }

private:
  // 各个iterator的范围是delta_个值
  // 各个iterator直接重叠overlap_个值
  // 举例
  // 假设overlap_=3, delta_=8, 当前RangeR: [-10, 15), 则:
  // begin iter: [-10, -2), delta_: 8
  // next1 iter: [-5, 3), delta_: 8, 与begin iter重叠了3个值: [-5, -4, -3]
  // next2 Iter: [0, 8), delta_: 8, 与next1 iter重叠了3个值: [0, 1, 2]
  // next3 Iter: [5, 13), delta_: 8, 与next2 iter重叠了3个值: [5, 6, 7]
  // next4 Iter: [10, 15), delta_: 5, 与next3 iter重叠了3个值: [10, 11, 12]
  // 且next4 iter不是一个完整的delta_范围, 被缩减至适应范围了
  // end Iter: [15, 15), delta_: 0, 不含有效范围
  T begin_;
  T end_;
  T delta_;
  T overlap_;
};

template <typename TROW, typename TCOL = TROW> class RRange2DIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::size_t;
  using value_type = Range2D<TROW, TCOL>;
  using reference = Range2D<TROW, TCOL> &;
  using pointer = Range2D<TROW, TCOL> *;

  // 构造函数
  RRange2DIterator(TROW begin_row, TROW end_row, TCOL begin_col, TCOL end_col,
                   const RRange2D<TROW, TCOL> *container)
      : val(begin_row, end_row, begin_col, end_col), container_(container) {}

  bool operator==(const RRange2DIterator &it) const { return !(*this != it); }

  bool operator!=(const RRange2DIterator &it) const { return val != it.val; }

  RRange2DIterator &operator++() {
    auto begin_row = val.row_.begin_;
    auto end_row = val.row_.end_;

    auto begin_col = val.col_.begin_;
    auto end_col = val.col_.end_;

    typename RRange<TROW>::iterator row_iter(begin_row, end_row,
                                             &container_->row_);
    typename RRange<TCOL>::iterator col_iter(begin_col, end_col,
                                             &container_->col_);

    if (end_row == container_->row_.end_ && end_col == container_->col_.end_) {
      // this is the last, the next is RRange2D::end()
      (*this) = container_->cend();
    } else {
      //  the next is normal

      ++col_iter;

      // next row
      if (col_iter == container_->col_.cend()) {
        // reset col index
        col_iter = container_->col_.cbegin();
        // increment row index if not end of rrange2d
        ++row_iter;
      }

      val.row_ = *row_iter;
      val.col_ = *col_iter;
    }

    return *this;
  }

  RRange2DIterator operator++(int) {
    RRange2DIterator tmp = *this;
    ++*this;
    return tmp;
  }

  value_type &operator*() { return val; }

private:
  value_type val;
  const RRange2D<TROW, TCOL> *container_;
};

template <typename TROW, typename TCOL = TROW> class RRange2D {
  friend class RRange2DIterator<TROW, TCOL>;

public:
  using iterator = RRange2DIterator<TROW, TCOL>;

public:
  RRange2D() {}
  RRange2D(TROW begin_row, TROW end_row, TROW delta_row, TROW overlap_row,
           TCOL begin_col, TCOL end_col, TCOL delta_col, TCOL overlap_col)
      : row_(begin_row, end_row, delta_row, overlap_row),
        col_(begin_col, end_col, delta_col, overlap_col) {}

  inline iterator cbegin() const {
    return empty()
               ? cend()
               : iterator(row_.begin_,
                          std::min(row_.begin_ + row_.delta_, row_.end_),
                          col_.begin_,
                          std::min(col_.begin_ + col_.delta_, col_.end_), this);
  }
  inline iterator begin() {
    return empty()
               ? end()
               : iterator(row_.begin_,
                          std::min(row_.begin_ + row_.delta_, row_.end_),
                          col_.begin_,
                          std::min(col_.begin_ + col_.delta_, col_.end_), this);
  }

  inline iterator cend() const {
    return iterator(row_.end_, row_.end_, col_.end_, col_.end_, this);
  }
  inline iterator end() {
    return iterator(row_.end_, row_.end_, col_.end_, col_.end_, this);
  }

  inline typename iterator::difference_type size() const {
    return std::distance(cbegin(), cend());
  }
  inline bool empty() const { return row_.empty() || col_.empty(); }

  // inline bool operator==(const RRange2D &rhs) const { return !(*this != rhs);
  // }

  // inline bool operator!=(const RRange2D &rhs) const {
  //   return row_ != rhs.row_ || col_ != rhs.col_;
  // }

private:
  RRange<TROW> row_;
  RRange<TCOL> col_;
};
} // namespace fpa

#endif // FPA_RANGE_H