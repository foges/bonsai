#ifndef BONSAI__IMPL__BRANCH_H_
#define BONSAI__IMPL__BRANCH_H_

#include <array>
#include <cmath>

#include "impl/data-types.h"

namespace bonsai {
namespace impl {
namespace internal {

template <typename IndexType, typename FloatType>
IndexType find_max_diff_iter(const Solution<FloatType> &solution,
                             const std::vector<IndexType> &integer_indices) {
  IndexType max_diff_iter;
  FloatType max_abs_diff = -1.0;
  for (IndexType iter = 0; iter < integer_indices.size(); ++iter) {
    const auto val = solution.x[integer_indices[iter]];
    const auto diff = std::abs(std::round(val) - val);
    if (diff > max_abs_diff) {
      max_abs_diff = diff;
      max_diff_iter = iter;
    }
  }
  return max_diff_iter;
}

} // namespace internal

template <typename IndexType, typename FloatType>
std::array<Branch<FloatType>, 2>
branch(Work<IndexType, FloatType> &work,
       const std::vector<Bound<FloatType>> &integer_bounds,
       const Solution<FloatType> &solution) {
  IndexType split_iter =
      internal::find_max_diff_iter(solution, work.integer_indices);
  auto integer_bounds_lower = integer_bounds;
  auto integer_bounds_upper = integer_bounds;
  integer_bounds_lower[split_iter].upper_bound =
      std::floor(solution.x[work.integer_indices[split_iter]]);
  integer_bounds_upper[split_iter].lower_bound =
      std::ceil(solution.x[work.integer_indices[split_iter]]);

  FloatType objective = solution.primal_objective;
  return {Branch<FloatType>{.priority = objective,
                            .lower_bound = objective,
                            .integer_bounds = integer_bounds_lower},
          Branch<FloatType>{.priority = objective,
                            .lower_bound = objective,
                            .integer_bounds = integer_bounds_upper}};
}

} // namespace impl
} // namespace bonsai

#endif // BONSAI__IMPL__BRANCH_H_