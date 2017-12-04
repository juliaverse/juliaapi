#ifndef XPTR_H
#define XPTR_H

#include "../inst/include/julia.h"
#include <Rcpp.h>

using namespace julia;

SEXP cast_xptr(jl_value_t* s, bool preserve = true);
jl_value_t* cast_jl_value_t(SEXP s);

#endif
