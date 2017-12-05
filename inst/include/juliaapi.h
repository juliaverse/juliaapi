#ifndef JULIAAPI_H
#define JULIAAPI_H

#include "julia.h"

#include <RcppCommon.h>

namespace Rcpp {
    SEXP wrap(jl_value_t* object);
    template<> jl_value_t* as(SEXP object);
}

#ifndef JULIAAPI_INIT
#define JL_EXTERN extern
#else
#define JL_EXTERN
#endif

#ifdef JULIAAPI_INTERNAL
    // load internally

    void juliaapi_check_exception();
    void juliaapi_print(jl_value_t* t);
    SEXP juliaapi_eval_string(const char* str, bool preserve);

#else
    // load externally

    JL_EXTERN void (*juliaapi_check_exception)();
    JL_EXTERN void (*juliaapi_print)(jl_value_t* t);
    JL_EXTERN SEXP (*juliaapi_eval_string)(const char* str, bool preserve);

#endif

#ifdef JULIAAPI_INIT
    #include "juliaapi_init.h"
#endif

#endif
