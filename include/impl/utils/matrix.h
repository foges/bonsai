#ifndef BONSAI__IMPL__UTILS__MATRIX_H_
#define BONSAI__IMPL__UTILS__MATRIX_H_

#include "data-types.h"
#include "impl/data-types.h"

namespace bonsai {
namespace impl {
namespace internal {

template <typename IndexType, typename FloatType>
std::tuple<std::vector<FloatType>, std::vector<IndexType>,
           std::vector<IndexType>>
matrix_transpose(const std::vector<FloatType> &values,
                 const std::vector<IndexType> &index,
                 const std::vector<IndexType> &pointer,
                 const IndexType slow_dim, const IndexType fast_dim) {
  // printvec(values, "values");
  // printvec(index, "index");
  // printvec(pointer, "pointer");
  // std::cout << slow_dim << " " << fast_dim << std::endl;
  std::vector<std::tuple<IndexType, IndexType, FloatType>> coo_matrix;
  for (IndexType slow_idx = 0; slow_idx < slow_dim; ++slow_idx) {
    const IndexType start = pointer[slow_idx];
    const IndexType end = pointer[slow_idx + 1];
    for (IndexType iter = start; iter < end; ++iter) {
      const IndexType fast_idx = index[iter];
      const FloatType val = values[iter];
      coo_matrix.push_back({fast_idx, slow_idx, val});
    }
  }

  std::sort(coo_matrix.begin(), coo_matrix.end());

  std::vector<FloatType> values_transposed;
  std::vector<IndexType> index_transposed;
  std::vector<IndexType> pointer_transposed;

  for (IndexType iter = 0; iter < coo_matrix.size(); ++iter) {
    const auto &element = coo_matrix[iter];
    const IndexType curr_idx = std::get<0>(element);
    while (pointer_transposed.size() < curr_idx + 1) {
      pointer_transposed.push_back(values_transposed.size());
    }
    index_transposed.push_back(std::get<1>(element));
    values_transposed.push_back(std::get<2>(element));
  }
  while (pointer_transposed.size() < fast_dim + 1) {
    pointer_transposed.push_back(values_transposed.size());
  }
  return {values_transposed, index_transposed, pointer_transposed};
}

} // namespace internal

template <typename IndexType, typename FloatType>
CscMatrix<IndexType, FloatType>
csr_to_csc_matrix(const Matrix<IndexType, FloatType> &matrix) {
  const auto [values, row_index, column_pointer] =
      internal::matrix_transpose(matrix.values, matrix.column_index,
                                 matrix.row_pointer, matrix.m, matrix.n);

  return CscMatrix<IndexType, FloatType>{.m = matrix.m,
                                         .n = matrix.n,
                                         .values = values,
                                         .row_index = row_index,
                                         .column_pointer = column_pointer};
}

template <typename IndexType, typename FloatType>
Matrix<IndexType, FloatType>
csc_to_csr_matrix(const CscMatrix<IndexType, FloatType> &matrix) {
  const auto [values, column_index, row_pointer] =
      internal::matrix_transpose(matrix.values, matrix.row_index,
                                 matrix.column_pointer, matrix.n, matrix.m);

  return Matrix<IndexType, FloatType>{.m = matrix.m,
                                      .n = matrix.n,
                                      .values = values,
                                      .column_index = column_index,
                                      .row_pointer = row_pointer};
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
             const IndexType row_split_location) {
  const auto &values = matrix.values;
  const auto &column_index = matrix.column_index;
  const auto &row_pointer = matrix.row_pointer;

  Matrix<IndexType, FloatType> matrix_a{
      .m = row_split_location,
      .n = matrix.n,
      .values = std::vector<FloatType>(
          values.data(), values.data() + row_pointer[row_split_location]),
      .column_index = std::vector<IndexType>(
          column_index.data(),
          column_index.data() + row_pointer[row_split_location]),
      .row_pointer = std::vector<IndexType>(
          row_pointer.data(), row_pointer.data() + row_split_location + 1)};
  Matrix<IndexType, FloatType> matrix_b{
      .m = matrix.m - row_split_location,
      .n = matrix.n,
      .values = std::vector<FloatType>(values.data() +
                                           row_pointer[row_split_location],
                                       values.data() + values.size()),
      .column_index = std::vector<IndexType>(
          column_index.data() + row_pointer[row_split_location],
          column_index.data() + column_index.size()),
      .row_pointer =
          std::vector<IndexType>(row_pointer.data() + row_split_location,
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
