#ifndef BONSAI__IMPL__PRESOLVE_H_
#define BONSAI__IMPL__PRESOLVE_H_

#include <cmath>

#include "data-types.h"
#include "impl/data-types.h"

namespace bonsai {
namespace impl {
namespace internal {

template <typename FloatType>
void round_zeros(std::vector<FloatType> &values, const Settings &settings) {
  for (auto &val : values) {
    if (std::fabs(val) < settings.eps) {
      val = 0.0;
    }
  }
}

template <typename IndexType, typename FloatType>
void remove_solitary_row(Work<IndexType, FloatType> &work,
                         const IndexType row_idx, const Settings &settings) {
  Matrix<IndexType, FloatType> &A = work.problem_data.positive_cone_A;
  std::vector<FloatType> &b = work.problem_data.positive_cone_b;
  std::vector<Bound<FloatType>> &initial_integer_bounds =
      work.initial_branch.integer_bounds;
  const std::vector<bool> &is_integer = work.is_integer;

  const IndexType start = A.row_pointer[row_idx];
  const IndexType end = A.row_pointer[row_idx + 1];
  bool found_non_zero_int = false;
  IndexType non_zero_iter = -1;
  for (IndexType col_iter = start; col_iter < end; ++col_iter) {
    const IndexType col_idx = A.column_index[col_iter];
    if (A.values[col_iter] != 0.0) {
      if (!is_integer[col_idx] || found_non_zero_int) {
        return;
      }
      found_non_zero_int = true;
      non_zero_iter = col_iter;
    }
  }
  if (!found_non_zero_int) {
    return;
  }
  auto &boundaries = initial_integer_bounds[A.column_index[non_zero_iter]];
  const FloatType new_bound = b[row_idx] / A.values[non_zero_iter];
  if (A.values[non_zero_iter] > 0) {
    boundaries.lower_bound =
        std::max(boundaries.lower_bound, std::floor(new_bound + settings.eps));
  } else {
    boundaries.upper_bound =
        std::min(boundaries.upper_bound, std::ceil(new_bound - settings.eps));
  }
  A.values[non_zero_iter] = 0;
  b[row_idx] = 0;
}

} // namespace internal

template <typename IndexType, typename FloatType>
void presolve(Work<IndexType, FloatType> &work, const Settings &settings) {
  internal::round_zeros(work.problem_data.positive_cone_A.values, settings);
  for (IndexType row_idx = 0; row_idx < work.problem_data.positive_cone_A.m;
       ++row_idx) {
    internal::remove_solitary_row(work, row_idx, settings);
  }
}

} // namespace impl
} // namespace bonsai

#endif // BONSAI__IMPL__PRESOLVE_H_