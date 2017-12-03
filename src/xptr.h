#ifndef __XPTR_HPP__
#define __XPTR_HPP__

#include <Rcpp.h>
#include "libjulia.h"

using namespace libjulia;

SEXP cast_xptr(jl_value_t* s, bool preserve = true);
jl_value_t* cast_jl_value_t(SEXP s);

#endif
