#include <cmath>
#include <iterator>

#include "bonsai-ecos.h"
#include "unittest.h"

void test_basic() {
  idxint n = 2;
  idxint m = 2;
  pfloat feas_Gx[4] = {2.0, 3.0, 1.0, 4.0};
  idxint feas_Gp[3] = {0, 2, 4};
  idxint feas_Gi[4] = {0, 1, 0, 1};

  pfloat feas_c[2] = {-1.1, -1.};
  pfloat feas_h[2] = {4., 12.};

  idxint bool_idx[1] = {0};

  pfloat x[2] = {1.0, 2.0};

  BonsaiEcosSolution solution =
      bonsai_ecos_solve(n, m, 0, m, 0, NULL, 0, feas_Gx, feas_Gp, feas_Gi, NULL,
                        NULL, NULL, feas_c, feas_h, NULL, 1, bool_idx, 0, NULL);
  const auto x_sol = std::vector(solution.x, solution.x + solution.n);
  ASSERT_VEC_EQ_EPS(x, x_sol, 1e-5);
  bonsai_ecos_cleanup(solution);
}

void test_1a_bool() {
  idxint n = 2;
  idxint m = 2;
  pfloat feas_Gx[4] = {2.0, 3.0, 1.0, 4.0};
  idxint feas_Gp[3] = {0, 2, 4};
  idxint feas_Gi[4] = {0, 1, 0, 1};

  pfloat feas_c[2] = {-1.1, -1.};
  pfloat feas_h[2] = {4., 12.};

  idxint bool_idx[1] = {0};

  /* Answer: */
  pfloat x[2] = {1.0, 2.0};

  BonsaiEcosSolution solution =
      bonsai_ecos_solve(n, m, 0, m, 0, NULL, 0, feas_Gx, feas_Gp, feas_Gi, NULL,
                        NULL, NULL, feas_c, feas_h, NULL, 1, bool_idx, 0, NULL);

  const auto x_sol = std::vector(solution.x, solution.x + solution.n);
  ASSERT_VEC_EQ_EPS(x, x_sol, 1e-5);
  bonsai_ecos_cleanup(solution);
}

void test_1a_int() {
  idxint n = 2;
  idxint m = 4;
  pfloat feas_Gx[6] = {2.0, 3.0, -1, 1.0, 4.0, -1};
  idxint feas_Gp[3] = {0, 3, 6};
  idxint feas_Gi[6] = {0, 1, 2, 0, 1, 3};

  pfloat feas_c[2] = {-1., -1.1};
  pfloat feas_h[4] = {4., 12., 0., 0.};

  idxint int_idx[2] = {0, 1};

  /* Answer: */
  pfloat x[2] = {0.0, 3.0};

  BonsaiEcosSolution solution =
      bonsai_ecos_solve(n, m, 0, m, 0, NULL, 0, feas_Gx, feas_Gp, feas_Gi, NULL,
                        NULL, NULL, feas_c, feas_h, NULL, 0, NULL, 2, int_idx);

  const auto x_sol = std::vector(solution.x, solution.x + solution.n);
  ASSERT_VEC_EQ_EPS(x, x_sol, 1e-5);
  bonsai_ecos_cleanup(solution);
}

void test_1b() {
  idxint n = 2;
  idxint m = 2;
  pfloat feas_Gx[4] = {2.0, 3.0, 1.0, 4.0};
  idxint feas_Gp[3] = {0, 2, 4};
  idxint feas_Gi[4] = {0, 1, 0, 1};

  pfloat feas_c[2] = {-1., -1.};
  pfloat feas_h[2] = {4., 12.};

  idxint bool_idx[1] = {1};

  /* Answer: */
  pfloat x[2] = {1.5, 1.0};

  BonsaiEcosSolution solution =
      bonsai_ecos_solve(n, m, 0, m, 0, NULL, 0, feas_Gx, feas_Gp, feas_Gi, NULL,
                        NULL, NULL, feas_c, feas_h, NULL, 1, bool_idx, 0, NULL);

  const auto x_sol = std::vector(solution.x, solution.x + solution.n);
  ASSERT_VEC_EQ_EPS(x, x_sol, 1e-5);
  bonsai_ecos_cleanup(solution);
}

void test_2() {
  idxint n = 2;
  idxint m = 2;
  pfloat feas_Gx[4] = {2.0, 3.0, 1.0, 4.0};
  idxint feas_Gp[3] = {0, 2, 4};
  idxint feas_Gi[4] = {0, 1, 0, 1};

  pfloat feas_c[2] = {-1., -1.};
  pfloat feas_h[2] = {4., 12.};

  idxint bool_idx[2] = {0, 1};

  /* Answer: */
  pfloat x[2] = {1.0, 1.0};

  BonsaiEcosSolution solution =
      bonsai_ecos_solve(n, m, 0, m, 0, NULL, 0, feas_Gx, feas_Gp, feas_Gi, NULL,
                        NULL, NULL, feas_c, feas_h, NULL, 2, bool_idx, 0, NULL);

  const auto x_sol = std::vector(solution.x, solution.x + solution.n);
  ASSERT_VEC_EQ_EPS(x, x_sol, 1e-5);
  bonsai_ecos_cleanup(solution);
}

void test_3() {
  idxint n = 2;
  idxint m = 2;
  pfloat feas_Gx[4] = {2.0, 3.0, 1.0, 4.0};
  idxint feas_Gp[3] = {0, 2, 4};
  idxint feas_Gi[4] = {0, 1, 0, 1};

  pfloat feas_c[2] = {1., -1.};
  pfloat feas_h[2] = {4., 12.};

  idxint bool_idx[1] = {0};

  /* Answer: */
  pfloat x[2] = {0.0, 3.0};

  BonsaiEcosSolution solution =
      bonsai_ecos_solve(n, m, 0, m, 0, NULL, 0, feas_Gx, feas_Gp, feas_Gi, NULL,
                        NULL, NULL, feas_c, feas_h, NULL, 1, bool_idx, 0, NULL);

  const auto x_sol = std::vector(solution.x, solution.x + solution.n);
  ASSERT_VEC_EQ_EPS(x, x_sol, 1e-5);
  bonsai_ecos_cleanup(solution);
}

void test_4() {
  idxint n = 6;
  idxint m = 3;
  pfloat feas_Gx[18] = {2,  5,  -5, -6, 3, 1,  3, -1, -4,
                        -4, -3, 2,  -1, 2, -2, 2, -1, 1};
  idxint feas_Gp[7] = {0, 3, 6, 9, 12, 15, 18};
  idxint feas_Gi[18] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2};

  pfloat feas_c[6] = {3, 5, 6, 9, 10, 10};
  pfloat feas_h[3] = {-2, 2, -3};

  idxint bool_idx[6] = {0, 1, 2, 3, 4, 5};

  /* Answer: */
  pfloat x[6] = {0, 1, 1, 0, 0, 0};

  BonsaiEcosSolution solution =
      bonsai_ecos_solve(n, m, 0, m, 0, NULL, 0, feas_Gx, feas_Gp, feas_Gi, NULL,
                        NULL, NULL, feas_c, feas_h, NULL, 6, bool_idx, 0, NULL);

  const auto x_sol = std::vector(solution.x, solution.x + solution.n);
  ASSERT_VEC_EQ_EPS(x, x_sol, 1e-5);
  bonsai_ecos_cleanup(solution);
}

void test_5() {
  idxint n = 6;
  idxint m = 3;
  pfloat feas_Gx[18] = {2,  5,  -5, -6, 3, 1,  3, -1, -4,
                        -4, -3, 2,  -1, 2, -2, 2, -1, 1};
  idxint feas_Gp[7] = {0, 3, 6, 9, 12, 15, 18};
  idxint feas_Gi[18] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2};

  pfloat feas_c[6] = {3, 5, 6, 9, -10, -10};
  pfloat feas_h[3] = {-2, 2, -3};

  idxint bool_idx[6] = {0, 1, 2, 3, 4, 5};

  /* Answer: */
  pfloat x[6] = {0, 0, 1, 1, 1, 0};

  BonsaiEcosSolution solution =
      bonsai_ecos_solve(n, m, 0, m, 0, NULL, 0, feas_Gx, feas_Gp, feas_Gi, NULL,
                        NULL, NULL, feas_c, feas_h, NULL, 6, bool_idx, 0, NULL);

  const auto x_sol = std::vector(solution.x, solution.x + solution.n);
  ASSERT_VEC_EQ_EPS(x, x_sol, 1e-5);
  bonsai_ecos_cleanup(solution);
}

void test_6() {
  idxint n = 6;
  idxint m = 3;
  pfloat feas_Gx[18] = {2,  5,  -5, -6, 3, 1,  3, -1, -4,
                        -4, -3, 2,  -1, 2, -2, 2, -1, 1};
  idxint feas_Gp[7] = {0, 3, 6, 9, 12, 15, 18};
  idxint feas_Gi[18] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2};

  pfloat feas_c[6] = {-3, -5, 6, 9, 10, -10};
  pfloat feas_h[3] = {-2, 1, -3};

  idxint bool_idx[6] = {0, 1, 2, 3, 4, 5};

  /* Answer: */
  pfloat x[6] = {0, 1, 1, 1, 1, 0};

  BonsaiEcosSolution solution =
      bonsai_ecos_solve(n, m, 0, m, 0, NULL, 0, feas_Gx, feas_Gp, feas_Gi, NULL,
                        NULL, NULL, feas_c, feas_h, NULL, 6, bool_idx, 0, NULL);

  const auto x_sol = std::vector(solution.x, solution.x + solution.n);
  ASSERT_VEC_EQ_EPS(x, x_sol, 1e-5);
  bonsai_ecos_cleanup(solution);
}

int main() {
  test_basic();
  test_1a_bool();
  test_1a_int();
  test_1b();
  test_2();
  test_3();
  test_4();
  test_5();
  test_6();
  std::cout << "[ALL TESTS PASS]" << std::endl;
}