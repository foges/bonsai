#ifndef BONSAI__BONSAI_ECOS_H_
#define BONSAI__BONSAI_ECOS_H_

#include "ecos.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  pfloat *x;
  pfloat *y;
  pfloat *s;

  idxint n;
  idxint m;

  pfloat primal_objective;

  int status;
} BonsaiEcosSolution;

BonsaiEcosSolution
bonsai_ecos_solve(idxint n, idxint m, idxint p, idxint l, idxint ncones,
                  idxint *q, idxint nex, pfloat *Gpr, idxint *Gjc, idxint *Gir,
                  pfloat *Apr, idxint *Ajc, idxint *Air, pfloat *c, pfloat *h,
                  pfloat *b, idxint num_bool_vars, idxint *bool_vars_idx,
                  idxint num_int_vars, idxint *int_vars_idx);

void bonsai_ecos_cleanup(const BonsaiEcosSolution &ecos_solution);

#ifdef __cplusplus
}
#endif

#endif // BONSAI__BONSAI_ECOS_H_