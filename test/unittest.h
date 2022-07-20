#ifndef BONSAI_UNITTEST_H_
#define BONSAI_UNITTEST_H_

#include <stdio.h>

#include <iostream>
#include <iterator>
#include <vector>

#define __RESET "\033[0m" /* Default */
#define __RED "\033[31m"  /* Red */
#define __BLUE "\033[34m" /* Blue */

/* SCALAR ******************************************************************* */

#define ASSERT(statement)                                                      \
  do {                                                                         \
    if (!(statement)) {                                                        \
      std::cout << __FILE__ << ":" << __LINE__ << ":" << __BLUE << __func__    \
                << "\n"                                                        \
                << __RED << "ASSERT_FAILED" << __RESET << std::endl;           \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define ASSERT_EQ(a, b)                                                        \
  do {                                                                         \
    if ((a) != (b)) {                                                          \
      std::cout << __FILE__ << ":" << __LINE__ << ":" << __BLUE << __func__    \
                << "\n"                                                        \
                << __RED << "ASSERT_FAILED: " << (a) << " == " << (b)          \
                << __RESET << std::endl;                                       \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define ASSERT_EQ_EPS(a, b, eps)                                               \
  do {                                                                         \
    if ((a) - (b) > eps || (b) - (a) > eps) {                                  \
      std::cout << __FILE__ << ":" << __LINE__ << ":" << __BLUE << __func__    \
                << "\n"                                                        \
                << __RED << "ASSERT_FAILED: " << (a) << " == " << (b)          \
                << __RESET << std::endl;                                       \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define ASSERT_GEQ(a, b)                                                       \
  do {                                                                         \
    if ((a) < (b)) {                                                           \
      std::cout << __FILE__ << ":" << __LINE__ << ":" << __BLUE << __func__    \
                << "\n"                                                        \
                << __RED << "ASSERT_FAILED: " << (a) << " >= " << (b)          \
                << __RESET << std::endl;                                       \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define ASSERT_GT(a, b)                                                        \
  do {                                                                         \
    if ((a) <= (b)) {                                                          \
      std::cout << __FILE__ << ":" << __LINE__ << ":" << __BLUE << __func__    \
                << "\n"                                                        \
                << __RED << "ASSERT_FAILED: " << (a) << " > " << (b)           \
                << __RESET << std::endl;                                       \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define ASSERT_LEQ(a, b)                                                       \
  do {                                                                         \
    if ((a) > (b)) {                                                           \
      std::cout << __FILE__ << ":" << __LINE__ << ":" << __BLUE << __func__    \
                << "\n"                                                        \
                << __RED << "ASSERT_FAILED: " << (a) << " <= " << (b)          \
                << __RESET << std::endl;                                       \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define ASSERT_LT(a, b)                                                        \
  do {                                                                         \
    if ((a) >= (b)) {                                                          \
      std::cout << __FILE__ << ":" << __LINE__ << ":" << __BLUE << __func__    \
                << "\n"                                                        \
                << __RED << "ASSERT_FAILED: " << (a) << " < " << (b)           \
                << __RESET << std::endl;                                       \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define ASSERT_NEQ(a, b)                                                       \
  do {                                                                         \
    if ((a) == (b)) {                                                          \
      std::cout << __FILE__ << ":" << __LINE__ << ":" << __BLUE << __func__    \
                << "\n"                                                        \
                << __RED << "ASSERT_FAILED: " << (a) << " != " << (b)          \
                << __RESET << std::endl;                                       \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define ASSERT(statement)                                                      \
  do {                                                                         \
    if (!(statement)) {                                                        \
      std::cout << __FILE__ << ":" << __LINE__ << ":" << __BLUE << __func__    \
                << "\n"                                                        \
                << __RED << "ASSERT_FAILED" << __RESET << std::endl;           \
      return;                                                                  \
    }                                                                          \
  } while (0)

/* VECTOR ******************************************************************* */

#define ASSERT_VEC_EQ(a, b)                                                    \
  do {                                                                         \
    const auto vec_a = std::vector(std::begin(a), std::end(a));                \
    const auto vec_b = std::vector(std::begin(b), std::end(b));                \
    ASSERT_EQ(vec_a.size(), vec_b.size());                                     \
    size_t size = vec_a.size();                                                \
    for (size_t i = 0; i < size; ++i) {                                        \
      ASSERT_EQ(vec_a[i], vec_b[i]);                                           \
    }                                                                          \
  } while (0)

#define ASSERT_VEC_EQ_EPS(a, b, eps)                                           \
  do {                                                                         \
    const auto vec_a = std::vector(std::begin((a)), std::end((a)));            \
    const auto vec_b = std::vector(std::begin((b)), std::end((b)));            \
    ASSERT_EQ(vec_a.size(), vec_b.size());                                     \
    size_t size = vec_a.size();                                                \
    for (size_t i = 0; i < size; ++i) {                                        \
      ASSERT_EQ_EPS(vec_a[i], vec_b[i], (eps));                                \
    }                                                                          \
  } while (0)

#endif // BONSAI_UNITTEST_H_