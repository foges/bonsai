#ifndef BONSAI__IMPL__INIT_H_
#define BONSAI__IMPL__INIT_H_

#include <limits>

#include "data-types.h"
#include "impl/data-types.h"

namespace bonsai {
namespace impl {
namespace internal {

template <typename IndexType, typename FloatType>
std::tuple<std::vector<Bound<FloatType>>, std::vector<IndexType>>
create_initial_bounds(
    const IntegralConstraints<IndexType> &integral_constraints,
    const Settings &settings) {
  std::vector<Bound<FloatType>> integer_bounds;
  std::vector<IndexType> integer_indices;
  for (const auto bool_idx : integral_constraints.bool_indices) {
    integer_indices.push_back(bool_idx);
    integer_bounds.push_back({0.0, 1.0});
  }
  for (const auto integer_idx : integral_constraints.integer_indices) {
    integer_indices.push_back(integer_idx);
    integer_bounds.push_back({-settings.int_max, settings.int_max});
  }
  return {integer_bounds, integer_indices};
}

template <typename IndexType>
std::vector<bool>
create_integer_map(const std::vector<IndexType> &integer_indices,
                   const int num_variables) {
  std::vector<bool> is_integer(num_variables);
  for (const auto index : integer_indices) {
    is_integer[index] = true;
  }
  return is_integer;
}

} // namespace internal

template <typename IndexType, typename FloatType>
Work<IndexType, FloatType>
init(const ProblemData<IndexType, FloatType> &problem_data,
     const IntegralConstraints<IndexType> &integral_constraints,
     const Settings &settings) {
  const auto [integer_bounds, integer_indices] =
      internal::create_initial_bounds<IndexType, FloatType>(
          integral_constraints, settings);
  const auto is_integer =
      internal::create_integer_map(integer_indices, problem_data.c.size());

  return Work<IndexType, FloatType>{
      .problem_data = problem_data,
      .is_integer = is_integer,
      .integer_indices = integer_indices,
      .initial_branch = {settings.int_max, settings.int_max, integer_bounds},
      .best_solution = {.status = kInfeasible,
                        .primal_objective =
                            std::numeric_limits<double>::infinity()}};
}

} // namespace impl
} // namespace bonsai

#endif // BONSAI__IMPL__INIT_H_