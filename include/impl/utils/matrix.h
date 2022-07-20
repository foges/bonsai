#ifndef BONSAI__IMPL__UTILS__MATRIX_H_
#define BONSAI__IMPL__UTILS__MATRIX_H_

#include <iostream>

#include "data-types.h"

namespace bonsai {
namespace impl {

template <typename IndexType, typename FloatType>
CscMatrix<IndexType, FloatType>
csr_to_csc_matrix(const Matrix<IndexType, FloatType> &matrix) {
  std::vector<std::tuple<IndexType, IndexType, FloatType>> coo_matrix;
  for (IndexType row_idx = 0; row_idx < matrix.m; ++row_idx) {
    const IndexType start = matrix.row_pointer[row_idx];
    const IndexType end = matrix.row_pointer[row_idx + 1];
    for (IndexType iter = start; iter < end; ++iter) {
      const IndexType col_idx = matrix.column_index[iter];
      const FloatType val = matrix.values[iter];
      coo_matrix.push_back({col_idx, row_idx, val});
    }
  }

  std::sort(coo_matrix.begin(), coo_matrix.end());

  CscMatrix<IndexType, FloatType> csc_matrix{.m = matrix.m, .n = matrix.n};

  for (IndexType iter = 0; iter < coo_matrix.size(); ++iter) {
    const auto &element = coo_matrix[iter];
    const IndexType curr_col = std::get<0>(element);
    while (csc_matrix.column_pointer.size() < curr_col + 1) {
      csc_matrix.column_pointer.push_back(csc_matrix.values.size());
    }
    csc_matrix.row_index.push_back(std::get<1>(element));
    csc_matrix.values.push_back(std::get<2>(element));
  }
  while (csc_matrix.column_pointer.size() < matrix.n + 1) {
    csc_matrix.column_pointer.push_back(csc_matrix.values.size());
  }
  return csc_matrix;
}

template <typename IndexType, typename FloatType>
Matrix<IndexType, FloatType>
csc_to_csr_matrix(const CscMatrix<IndexType, FloatType> &matrix) {
  std::vector<std::tuple<IndexType, IndexType, FloatType>> coo_matrix;
  for (IndexType col_idx = 0; col_idx < matrix.n; ++col_idx) {
    const IndexType start = matrix.column_pointer[col_idx];
    const IndexType end = matrix.column_pointer[col_idx + 1];
    for (IndexType iter = start; iter < end; ++iter) {
      const IndexType row_idx = matrix.row_index[iter];
      const FloatType val = matrix.values[iter];
      coo_matrix.push_back({row_idx, col_idx, val});
    }
  }

  std::sort(coo_matrix.begin(), coo_matrix.end());

  Matrix<IndexType, FloatType> csr_matrix{.m = matrix.m, .n = matrix.n};

  for (IndexType iter = 0; iter < coo_matrix.size(); ++iter) {
    const auto &element = coo_matrix[iter];
    const IndexType curr_row = std::get<0>(element);
    while (csr_matrix.row_pointer.size() < curr_row + 1) {
      csr_matrix.row_pointer.push_back(csr_matrix.values.size());
    }
    csr_matrix.column_index.push_back(std::get<1>(element));
    csr_matrix.values.push_back(std::get<2>(element));
  }
  while (csr_matrix.row_pointer.size() < matrix.m + 1) {
    csr_matrix.row_pointer.push_back(csr_matrix.values.size());
  }
  return csr_matrix;
}

template <typename IndexType, typename FloatType>
Matrix<IndexType, FloatType>
vertical_concat(const Matrix<IndexType, FloatType> &matrix_a,
                const Matrix<IndexType, FloatType> &matrix_b) {
  assert(matrix_a.n == matrix_b.n);

  Matrix<IndexType, FloatType> matrix_c = matrix_a;
  matrix_c.m = matrix_a.m + matrix_b.m;

  matrix_c.values.insert(matrix_c.values.end(), matrix_b.values.begin(),
                         matrix_b.values.end());
  matrix_c.column_index.insert(matrix_c.column_index.end(),
                               matrix_b.column_index.begin(),
                               matrix_b.column_index.end());

  const auto row_pointer_start = matrix_c.row_pointer.back();
  for (IndexType iter = 1; iter < matrix_b.row_pointer.size(); ++iter) {
    matrix_c.row_pointer.push_back(row_pointer_start +
                                   matrix_b.row_pointer[iter]);
  }
  return matrix_c;
}

template <typename IndexType, typename FloatType>
std::tuple<Matrix<IndexType, FloatType>, Matrix<IndexType, FloatType>>
matrix_split(const Matrix<IndexType, FloatType> &matrix,
             const IndexType row_split_size) {
  const auto &values = matrix.values;
  const auto &column_index = matrix.column_index;
  const auto &row_pointer = matrix.row_pointer;

  Matrix<IndexType, FloatType> matrix_a{
      .m = row_split_size,
      .n = matrix.n,
      .values = std::vector<FloatType>(
          values.data(), values.data() + row_pointer[row_split_size]),
      .column_index = std::vector<IndexType>(column_index.data(),
                                             column_index.data() +
                                                 row_pointer[row_split_size]),
      .row_pointer = std::vector<IndexType>(
          row_pointer.data(), row_pointer.data() + row_split_size + 1)};
  Matrix<IndexType, FloatType> matrix_b{
      .m = matrix.m - row_split_size,
      .n = matrix.n,
      .values =
          std::vector<FloatType>(values.data() + row_pointer[row_split_size],
                                 values.data() + values.size()),
      .column_index = std::vector<IndexType>(
          column_index.data() + row_pointer[row_split_size],
          column_index.data() + column_index.size()),
      .row_pointer =
          std::vector<IndexType>(row_pointer.data() + row_split_size,
                                 row_pointer.data() + row_pointer.size())};
  for (auto &row_ptr : matrix_b.row_pointer) {
    row_ptr -= matrix_b.row_pointer[0];
  }
  return {matrix_a, matrix_b};
}

template <typename FloatType>
std::vector<FloatType> concat(const std::vector<FloatType> &vector_a,
                              const std::vector<FloatType> &vector_b) {
  std::vector<FloatType> vector_c = vector_a;
  vector_c.insert(vector_c.end(), vector_b.begin(), vector_b.end());
  return vector_c;
}

template <typename FloatType>
std::tuple<std::vector<FloatType>, std::vector<FloatType>>
split(const std::vector<FloatType> &vector, const size_t split_size) {
  std::vector<FloatType> vector_a(vector.data(), vector.data() + split_size);
  std::vector<FloatType> vector_b(vector.data() + split_size,
                                  vector.data() + vector.size());
  return {vector_a, vector_b};
}

} // namespace impl
} // namespace bonsai

#endif // BONSAI__IMPL__UTILS__MATRIX_H_
