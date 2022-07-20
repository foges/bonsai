#ifndef BONSAI__DATA_TYPES_H_
#define BONSAI__DATA_TYPES_H_

#include <vector>

namespace bonsai {

/** Compressed-sparse row matrix format. */
template <typename IndexType, typename FloatType> struct Matrix {
  /** Number of rows. */
  IndexType m;
  /** Number of columns. */
  IndexType n;
  /** Matrix values, size: number of non-zeros. */
  std::vector<FloatType> values;
  /** Matrix column indices, size: number of non-zeros. */
  std::vector<IndexType> column_index;
  /** Matrix row pointers, size: `m+1`. */
  std::vector<IndexType> row_pointer;
};

template <typename IndexType, typename FloatType> struct ProblemData {
  /** Cost vector. */
  std::vector<FloatType> c;

  /** Zero cone */
  Matrix<IndexType, FloatType> zero_cone_A;
  std::vector<FloatType> zero_cone_b;

  /** Positive cone */
  Matrix<IndexType, FloatType> positive_cone_A;
  std::vector<FloatType> positive_cone_b;

  /** Second order cone */
  Matrix<IndexType, FloatType> second_order_cone_A;
  std::vector<FloatType> second_order_cone_b;
  std::vector<IndexType> second_order_cone_lengths;
};

template <typename IndexType> struct IntegralConstraints {
  /** Indices of integer variables. */
  std::vector<IndexType> integer_indices;
  /** Indices of boolean variables. */
  std::vector<IndexType> bool_indices;
};

enum Status { kSolved, kInfeasible, kUnbounded, kSolverProblem, kAbort };

template <typename FloatType> struct Solution {
  /** Primal variable. */
  std::vector<FloatType> x;
  /** Dual variable. */
  std::vector<FloatType> y;
  /** Slack variable. */
  std::vector<FloatType> s;

  /** Primal objective value. */
  FloatType primal_objective;

  /** Solver status on exit. */
  Status status;
};

struct Settings {
  /** Maximum number of branch-and-bound iterations. */
  int max_iter = 100;

  /** Tolerance for classifying a value as integer. */
  double eps = 1e-5;

  /** Value to consider infinite. */
  double int_max = 1'000'000;

  /** Round integers and re-solve. */
  bool check_rounded_solution = true;
};

} // namespace bonsai

#endif // BONSAI__DATA_TYPES_H_