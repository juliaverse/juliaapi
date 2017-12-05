#ifndef JULIAAPI_H
#define JULIAAPI_H

#include "julia.h"

#include <RcppCommon.h>

namespace Rcpp {
    SEXP wrap(jl_value_t* object);
    template<> jl_value_t* as(SEXP object);
}

#ifdef JULIAAPI_INIT
    #include "juliaapi_init.h"
#endif

#endif
