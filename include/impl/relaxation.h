#ifndef BONSAI__IMPL__RELAXATION_H_
#define BONSAI__IMPL__RELAXATION_H_

#include "data-types.h"
#include "impl/data-types.h"

namespace bonsai {
namespace impl {
namespace internal {

template <typename IndexType, typename FloatType>
std::tuple<Matrix<IndexType, FloatType>, std::vector<FloatType>>
clean_constriant_zeros(const Matrix<IndexType, FloatType> &cone_A,
                       const std::vector<FloatType> &cone_b) {
  Matrix<IndexType, FloatType> cone_A_clean{.m = 0, .n = cone_A.n};
  std::vector<FloatType> cone_b_clean;

  for (IndexType row_idx = 0; row_idx < cone_A.m; ++row_idx) {
    const IndexType start = cone_A.row_pointer[row_idx];
    const IndexType end = cone_A.row_pointer[row_idx + 1];
    bool first_nonzero_in_row = true;
    for (IndexType iter = start; iter < end; ++iter) {
      const IndexType col_idx = cone_A.column_index[iter];
      const FloatType val = cone_A.values[iter];
      if (val != 0) {
        if (first_nonzero_in_row) {
          cone_A_clean.row_pointer.push_back(cone_A_clean.values.size());
          cone_b_clean.push_back(cone_b[row_idx]);
          ++cone_A_clean.m;
          first_nonzero_in_row = false;
        }
        cone_A_clean.column_index.push_back(col_idx);
        cone_A_clean.values.push_back(val);
      }
    }
  }

  cone_A_clean.row_pointer.push_back(cone_A_clean.values.size());

  return {cone_A_clean, cone_b_clean};
}

template <typename IndexType, typename FloatType>
void add_constriant_row(Matrix<IndexType, FloatType> &cone_A,
                        std::vector<FloatType> &cone_b,
                        const IndexType integer_index, const FloatType val_A,
                        const FloatType val_b) {
  cone_A.row_pointer.push_back(cone_A.row_pointer.back() + 1);
  cone_A.column_index.push_back(integer_index);
  cone_A.values.push_back(val_A);
  ++cone_A.m;

  cone_b.push_back(val_b);
}

template <typename IndexType, typename FloatType>
std::tuple<Matrix<IndexType, FloatType>, std::vector<FloatType>,
           Matrix<IndexType, FloatType>, std::vector<FloatType>>
merge_cone_bounds(const Matrix<IndexType, FloatType> &zero_cone_A,
                  const std::vector<FloatType> &zero_cone_b,
                  const Matrix<IndexType, FloatType> &positive_cone_A,
                  const std::vector<FloatType> &positive_cone_b,
                  const std::vector<IndexType> &integer_indices,
                  const std::vector<Bound<FloatType>> &integer_bounds,
                  const Settings &settings) {
  auto [zero_cone_A_merged, zero_cone_b_merged] =
      clean_constriant_zeros(zero_cone_A, zero_cone_b);
  auto [positive_cone_A_merged, positive_cone_b_merged] =
      clean_constriant_zeros(positive_cone_A, positive_cone_b);

  for (int iter = 0; iter < integer_indices.size(); ++iter) {
    const auto &bound = integer_bounds[iter];
    const auto integer_index = integer_indices[iter];
    if (bound.lower_bound == bound.upper_bound) {
      add_constriant_row(zero_cone_A_merged, zero_cone_b_merged, integer_index,
                         1.0, bound.upper_bound);
    } else {
      if (bound.lower_bound > -settings.int_max) {
        add_constriant_row(positive_cone_A_merged, positive_cone_b_merged,
                           integer_index, 1.0, bound.upper_bound);
      }
      if (bound.upper_bound < settings.int_max) {
        add_constriant_row(positive_cone_A_merged, positive_cone_b_merged,
                           integer_index, -1.0, -bound.lower_bound);
      }
    }
  }
  return {zero_cone_A_merged, zero_cone_b_merged, positive_cone_A_merged,
          positive_cone_b_merged};
}

} // namespace internal

template <typename IndexType, typename FloatType>
ProblemData<IndexType, FloatType>
create_relaxation(const ProblemData<IndexType, FloatType> &problem_data,
                  const std::vector<IndexType> &integer_indices,
                  const std::vector<Bound<FloatType>> &integer_bounds,
                  const Settings &settings) {

  const auto [zero_cone_A, zero_cone_b, positive_cone_A, positive_cone_b] =
      internal::merge_cone_bounds(
          problem_data.zero_cone_A, problem_data.zero_cone_b,
          problem_data.positive_cone_A, problem_data.positive_cone_b,
          integer_indices, integer_bounds, settings);

  return ProblemData<IndexType, FloatType>{
      .c = problem_data.c,

      .zero_cone_A = zero_cone_A,
      .zero_cone_b = zero_cone_b,

      .positive_cone_A = positive_cone_A,
      .positive_cone_b = positive_cone_b,

      .second_order_cone_A = problem_data.second_order_cone_A,
      .second_order_cone_b = problem_data.second_order_cone_b,
      .second_order_cone_lengths = problem_data.second_order_cone_lengths,
  };
}

} // namespace impl
} // namespace bonsai

#endif // BONSAI__IMPL__RELAXATION_H_