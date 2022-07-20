#include "bonsai-ecos.h"

bool float_eqls(pfloat x, pfloat y) { return std::abs(x - y) < 1e-5; }

int test_basic() {
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

  idxint i, pass;

  BonsaiEcosSolution solution =
      bonsai_ecos_solve(n, m, 0, m, 0, NULL, 0, feas_Gx, feas_Gp, feas_Gi, NULL,
                        NULL, NULL, feas_c, feas_h, NULL, 1, bool_idx, 0, NULL);

  pass = 1;

  printf("Soln:\n");
  for (i = 0; i < n; ++i) {
    pass &= float_eqls(x[i], solution.x[i]);
    printf("%f vs. %f\n", x[i], solution.x[i]);
  }
  bonsai_ecos_cleanup(solution);
  return pass;
}

int main() { test_basic(); }