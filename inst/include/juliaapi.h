#ifndef JULIAAPI_H
#define JULIAAPI_H

#ifdef JULIAAPI_LOADING
    #include "libjulia.h"
#endif

#include "julia.h"

// make RcppExports.cpp not complain
using namespace julia;

#include <RcppCommon.h>

namespace Rcpp {
    SEXP wrap(jl_value_t* object);
    template<> jl_value_t* as(SEXP object);
}

// load externally
#if defined(JULIAAPI_LOADING) && !defined(JULIAAPI_CPP)

SEXP (*cast_xptr)(jl_value_t* s, bool preserve);
jl_value_t* (*cast_jl_value_t)(SEXP s);

namespace Rcpp {

    SEXP wrap(jl_value_t* object) {
        return cast_xptr(object, true);
    }

    template<> jl_value_t* as(SEXP object) {
        return cast_jl_value_t(object);
    }

}

void juliaapi_init() {
    libjulia::load_symbol("cast_xptr", (void**) &cast_xptr);
    libjulia::load_symbol("cast_jl_value_t", (void**) &cast_jl_value_t);
    libjulia::load_symbols();
    libjulia::load_constants();
}

#endif // load externally

#endif
