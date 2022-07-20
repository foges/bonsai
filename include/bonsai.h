#ifndef BONSAI__BONSAI_H_
#define BONSAI__BONSAI_H_

#include "data-types.h"
#include "impl/branch.h"
#include "impl/data-types.h"
#include "impl/init.h"
#include "impl/presolve.h"
#include "impl/relaxation.h"
#include "impl/utils/math.h"

namespace bonsai {

template <typename SolverType, typename IndexType, typename FloatType>
Solution<FloatType>
solve(const SolverType &solver,
      const ProblemData<IndexType, FloatType> &problem_data,
      const IntegralConstraints<IndexType> &integral_constraints,
      const Settings &settings) {
  impl::Work<IndexType, FloatType> work =
      impl::init(problem_data, integral_constraints, settings);
  impl::presolve(work, settings);

  std::priority_queue<impl::Branch<FloatType>> branch_queue;
  branch_queue.push(work.initial_branch);
  for (IndexType iter = 0; iter < settings.max_iter && !branch_queue.empty();
       ++iter) {
    const auto branch = branch_queue.top();
    branch_queue.pop();

    if (branch.lower_bound >= work.best_solution.primal_objective) {
      continue;
    }

    const auto relaxed_problem =
        impl::create_relaxation(work.problem_data, work.integer_indices,
                                branch.integer_bounds, settings);
    const auto relaxed_solution = solver.solve(relaxed_problem);

    if (relaxed_solution.status == kUnbounded) {
      return relaxed_solution;
    }
    if (relaxed_solution.status != kSolved ||
        relaxed_solution.primal_objective >=
            work.best_solution.primal_objective) {
      continue;
    }
    if (impl::is_integer_solution(relaxed_solution, work.integer_indices,
                                  settings)) {
      work.best_solution = relaxed_solution;
      continue;
    }
    if (settings.check_rounded_solution) {
      const auto rounded_integer_bounds =
          impl::round(relaxed_solution, work.integer_indices);
      const auto rounded_problem =
          impl::create_relaxation(work.problem_data, work.integer_indices,
                                  rounded_integer_bounds, settings);

      const auto rounded_solution = solver.solve(rounded_problem);
      if (rounded_solution.status == kSolved &&
          rounded_solution.primal_objective <
              work.best_solution.primal_objective) {
        work.best_solution = rounded_solution;
      }
    }
    const auto new_branches =
        impl::branch(work, branch.integer_bounds, relaxed_solution);
    for (const auto &branch : new_branches) {
      branch_queue.push(branch);
    }
  }
  return work.best_solution;
}

} // namespace bonsai

#endif // BONSAI__BONSAI_H_