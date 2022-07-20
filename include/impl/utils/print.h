#ifndef BONSAI__IMPL__UTILS__PRINT_H_
#define BONSAI__IMPL__UTILS__PRINT_H_

#include <iostream>
#include <vector>

#include "data-types.h"
#include "impl/data-types.h"

namespace bonsai {
namespace impl {

template <typename FloatType>
void printvec(const std::vector<impl::Bound<FloatType>> &vec,
              const std::string &name) {
  std::cout << name << " (" << vec.size() << " elements)" << std::endl;
  for (const auto &el : vec) {
    std::cout << "(" << el.lower_bound << ", " << el.upper_bound << ") ";
  }
  std::cout << std::endl;
}

template <typename T>
void printvec(const std::vector<T> &vec, const std::string &name) {
  std::cout << name << " (" << vec.size() << " elements)" << std::endl;
  for (const auto &el : vec) {
    std::cout << el << " ";
  }
  std::cout << std::endl;
}

} // namespace impl
} // namespace bonsai

#endif // BONSAI__IMPL__UTILS__PRINT_H_