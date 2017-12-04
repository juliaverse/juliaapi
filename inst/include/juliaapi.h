#ifndef JULIAAPI_H
#define JULIAAPI_H

#include "julia.h"

// make RcppExports.cpp not complain
using namespace julia;

#include <RcppCommon.h>
namespace Rcpp {
    SEXP wrap(jl_value_t* object);
    template<> jl_value_t* as(SEXP object);
}

#endif
