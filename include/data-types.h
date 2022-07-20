#ifndef BONSAI__DATA_TYPES_H_
#define BONSAI__DATA_TYPES_H_

#include <iostream>
#include <vector>

namespace bonsai {

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
  std::vector<IndexType> integer_indices;
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

  FloatType primal_objective;

  Status status;
};

struct Settings {
  int max_iter = 1000;
  double eps = 1e-5;
  double int_max = 1'000'000;

  bool verbose = false;

  bool check_rounded_solution = true;
};

template <typename T> void printvec(const T &vec, const std::string &name) {
  std::cout << name << "(" << vec.size() << " elements)" << std::endl;
  for (const auto &el : vec) {
    std::cout << el << " ";
  }
  std::cout << std::endl;
}

} // namespace bonsai

#endif // BONSAI__DATA_TYPES_H_