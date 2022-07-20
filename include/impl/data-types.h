#ifndef BONSAI__IMPL__DATA_TYPES_H_
#define BONSAI__IMPL__DATA_TYPES_H_

#include <iostream>
#include <queue>
#include <vector>

#include "data-types.h"

namespace bonsai {
namespace impl {

template <typename FloatType> struct Bound {
  FloatType lower_bound;
  FloatType upper_bound;
};

template <typename FloatType> struct Branch {
  double priority;
  double lower_bound;
  std::vector<Bound<FloatType>> integer_bounds;

  friend bool operator<(const Branch<FloatType> &lhs,
                        const Branch<FloatType> &rhs) {
    return lhs.priority < rhs.priority;
  }
};

template <typename IndexType, typename FloatType> struct Work {
  ProblemData<IndexType, FloatType> problem_data;
  std::vector<IndexType> integer_indices;
  std::vector<bool> is_integer;
  Branch<FloatType> initial_branch;

  std::priority_queue<Branch<FloatType>> branch_queue;

  Solution<FloatType> best_solution;
};

template <typename IndexType, typename FloatType> struct CscMatrix {
  IndexType m;
  IndexType n;

  std::vector<FloatType> values;
  std::vector<IndexType> row_index;
  std::vector<IndexType> column_pointer;
};

} // namespace impl
} // namespace bonsai

#endif // BONSAI__IMPL__DATA_TYPES_H_