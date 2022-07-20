#ifndef BONSAI__IMPL__DATA_TYPES_H_
#define BONSAI__IMPL__DATA_TYPES_H_

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

template <typename FloatType>
void printvec(const std::vector<impl::Bound<FloatType>> &vec,
              const std::string &name) {
  std::cout << name << "(" << vec.size() << " elements)" << std::endl;
  for (const auto &el : vec) {
    std::cout << "(" << el.lower_bound << ", " << el.upper_bound << ") ";
  }
  std::cout << std::endl;
}

} // namespace bonsai

#endif // BONSAI__IMPL__DATA_TYPES_H_