#include "ecos.h"

#include "bonsai-ecos.h"
#include "bonsai.h"
#include "impl/utils/matrix.h"

namespace bonsai {
namespace {

// template <typename T> void printvec(const T &vec, const std::string &name) {
//   std::cout << name << "(" << vec.size() << " elements)" << std::endl;
//   for (const auto &el : vec) {
//     std::cout << el << " ";
//   }
//   std::cout << std::endl;
// }

Status map_status(int ecos_exitcode) {
  switch (ecos_exitcode) {
  case ECOS_OPTIMAL:
  case (ECOS_OPTIMAL + ECOS_INACC_OFFSET):
    return kSolved;
  case ECOS_PINF:
  case (ECOS_PINF + ECOS_INACC_OFFSET):
    return kInfeasible;
  case ECOS_DINF:
  case (ECOS_DINF + ECOS_INACC_OFFSET):
    return kUnbounded;
  case ECOS_SIGINT:
    return kAbort;
  case ECOS_NUMERICS:
  case ECOS_MAXIT:
  case ECOS_OUTCONE:
  default:
    return kSolverProblem;
  }
};

struct EcosSolver {
  Solution<pfloat>
  solve(const ProblemData<idxint, pfloat> &problem_data) const {
    auto c = problem_data.c;

    auto A = impl::csr_to_csc_matrix(problem_data.zero_cone_A);
    auto b = problem_data.zero_cone_b;

    auto G = impl::csr_to_csc_matrix(impl::vertical_concat(
        problem_data.positive_cone_A, problem_data.second_order_cone_A));
    auto h = impl::concat(problem_data.positive_cone_b,
                          problem_data.second_order_cone_b);

    auto second_order_cone_lengths = problem_data.second_order_cone_lengths;

    idxint n = c.size();
    idxint m = h.size();
    idxint p = b.size();
    idxint l = problem_data.positive_cone_A.m;
    idxint ncones = second_order_cone_lengths.size();
    idxint *qptr = second_order_cone_lengths.data();
    idxint nex = 0;
    pfloat *Gpr = G.values.data();
    idxint *Gjc = G.column_pointer.data();
    idxint *Gir = G.row_index.data();
    pfloat *Apr = A.values.data();
    idxint *Ajc = A.column_pointer.data();
    idxint *Air = A.row_index.data();
    pfloat *cptr = c.data();
    pfloat *hptr = h.data();
    pfloat *bptr = b.data();

    pwork *work = ECOS_setup(n, m, p, l, ncones, qptr, nex, Gpr, Gjc, Gir, Apr,
                             Ajc, Air, cptr, hptr, bptr);
    work->stgs->verbose = 0;
    int exitcode = ECOS_solve(work);

    std::vector<pfloat> x(work->x, work->x + n);

    std::vector<pfloat> s(m + p);
    std::copy(work->s, work->s + m, s.data() + p);

    std::vector<pfloat> y(m + p);
    std::copy(work->y, work->y + p, y.data());
    std::copy(work->z, work->z + m, y.data() + p);

    Solution<pfloat> solution{.x = x,
                              .s = s,
                              .y = y,
                              .primal_objective = work->info->pcost,
                              .status = map_status(exitcode)};

    ECOS_cleanup(work, 0);

    return solution;
  }
};

impl::CscMatrix<idxint, pfloat> ptr_to_csc(const idxint m, const idxint n,
                                           const pfloat *pr, const idxint *jc,
                                           const idxint *ir) {
  if (pr && jc && ir) {
    return {.m = m,
            .n = n,
            .values = std::vector<pfloat>(pr, pr + jc[n]),
            .row_index = std::vector<idxint>(ir, ir + jc[n]),
            .column_pointer = std::vector<idxint>(jc, jc + n + 1)};
  }
  return {.m = 0, .n = n, .column_pointer = std::vector<idxint>(n + 1)};
}

template <typename T>
std::vector<T> ptr_to_vec(const T *ptr, const idxint size) {
  if (ptr) {
    return std::vector<T>(ptr, ptr + size);
  }
  return std::vector<T>();
}

} // namespace

Solution<pfloat>
ecos_solve(const ProblemData<idxint, pfloat> &problem_data,
           const IntegralConstraints<idxint> &integral_constraints,
           const Settings &settings) {
  return solve(EcosSolver(), problem_data, integral_constraints, settings);
}

} // namespace bonsai

#ifdef __cplusplus
extern "C" {
#endif

BonsaiEcosSolution
bonsai_ecos_solve(idxint n, idxint m, idxint p, idxint l, idxint ncones,
                  idxint *q, idxint nex, pfloat *Gpr, idxint *Gjc, idxint *Gir,
                  pfloat *Apr, idxint *Ajc, idxint *Air, pfloat *c, pfloat *h,
                  pfloat *b, idxint num_bool_vars, idxint *bool_vars_idx,
                  idxint num_int_vars, idxint *int_vars_idx) {
  std::vector<pfloat> c_vec = bonsai::ptr_to_vec(c, n);

  const bonsai::impl::CscMatrix<idxint, pfloat> A_csc =
      bonsai::ptr_to_csc(p, n, Apr, Ajc, Air);

  const bonsai::Matrix<idxint, pfloat> zero_cone_A =
      bonsai::impl::csc_to_csr_matrix(A_csc);

  const std::vector<pfloat> zero_cone_b = bonsai::ptr_to_vec(b, p);

  const bonsai::impl::CscMatrix<idxint, pfloat> G_csc =
      bonsai::ptr_to_csc(m, n, Gpr, Gjc, Gir);

  const auto G_csr = bonsai::impl::csc_to_csr_matrix(G_csc);

  const auto [positive_cone_A, second_order_cone_A] =
      bonsai::impl::matrix_split(G_csr, l);

  std::vector<pfloat> h_vec = bonsai::ptr_to_vec(h, m);

  const auto [positive_cone_b, second_order_cone_b] =
      bonsai::impl::split(h_vec, l);

  const std::vector<idxint> second_order_cone_lengths =
      bonsai::ptr_to_vec(q, ncones);

  const bonsai::ProblemData<idxint, pfloat> problem_data{
      .c = c_vec,

      .zero_cone_A = zero_cone_A,
      .zero_cone_b = zero_cone_b,

      .positive_cone_A = positive_cone_A,
      .positive_cone_b = positive_cone_b,

      .second_order_cone_A = second_order_cone_A,
      .second_order_cone_b = second_order_cone_b,
      .second_order_cone_lengths = second_order_cone_lengths};

  const bonsai::IntegralConstraints<idxint> integral_constraints{
      .integer_indices = bonsai::ptr_to_vec(int_vars_idx, num_int_vars),
      .bool_indices = bonsai::ptr_to_vec(bool_vars_idx, num_bool_vars)};

  const auto solution = bonsai::ecos_solve(problem_data, integral_constraints,
                                           bonsai::Settings());

  BonsaiEcosSolution ecos_solution;
  ecos_solution.x = reinterpret_cast<pfloat *>(
      malloc(solution.x.size() * sizeof(solution.x[0])));
  std::copy(solution.x.data(), solution.x.data() + solution.x.size(),
            ecos_solution.x);
  ecos_solution.y = reinterpret_cast<pfloat *>(
      malloc(solution.y.size() * sizeof(solution.y[0])));
  std::copy(solution.y.data(), solution.y.data() + solution.y.size(),
            ecos_solution.y);
  ecos_solution.s = reinterpret_cast<pfloat *>(
      malloc(solution.s.size() * sizeof(solution.s[0])));
  std::copy(solution.s.data(), solution.s.data() + solution.s.size(),
            ecos_solution.s);
  ecos_solution.n = n;
  ecos_solution.m = m;
  ecos_solution.primal_objective = solution.primal_objective;
  ecos_solution.status = solution.status;

  return ecos_solution;
}

void bonsai_ecos_cleanup(const BonsaiEcosSolution &ecos_solution) {
  free(ecos_solution.x);
  free(ecos_solution.y);
  free(ecos_solution.s);
}

#ifdef __cplusplus
}
#endif
