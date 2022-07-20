#ifndef BONSAI__IMPL__UTILS__MATH_H_
#define BONSAI__IMPL__UTILS__MATH_H_

#include <cmath>

namespace bonsai {
namespace impl {

template <typename IndexType, typename FloatType>
bool is_integer_solution(const Solution<FloatType> &relaxed_solution,
                         const std::vector<IndexType> &integer_indices,
                         const Settings &settings) {
  for (const IndexType col_idx : integer_indices) {
    const auto val = relaxed_solution.x[col_idx];
    FloatType diff = std::abs(std::round(val) - val);
    if (diff > settings.eps) {
      return false;
    }
  }
  return true;
}

template <typename IndexType, typename FloatType>
std::vector<Bound<FloatType>>
round(const Solution<FloatType> &relaxed_solution,
      const std::vector<IndexType> &integer_indices) {

  std::vector<Bound<FloatType>> rounded_integer_bounds;
  for (const auto int_idx : integer_indices) {
    const auto rounded_val = std::round(relaxed_solution.x[int_idx]);
    rounded_integer_bounds.push_back({rounded_val, rounded_val});
  }
  return rounded_integer_bounds;
}

} // namespace impl
} // namespace bonsai

#endif // BONSAI__IMPL__UTILS__MATH_H_