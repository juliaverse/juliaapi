#ifndef JULIAAPI_H
#define JULIAAPI_H

#include "julia.h"

#ifndef NO_RCPP
#include <RcppCommon.h>

namespace Rcpp {
    SEXP wrap(jl_value_t* object);
    template<> jl_value_t* as(SEXP object);
}
#else
#include <Rinternals.h>
#endif

#ifndef JULIAAPI_INIT
#define JL_EXTERN extern
#else
#define JL_EXTERN
#endif

#ifdef JULIAAPI_INTERNAL
    // load internally

    SEXP cast_xptr(jl_value_t* s, bool preserve);
    jl_value_t* cast_jl_value_t(SEXP s);

    bool juliaapi_init(const std::string& libpath);
    bool juliaapi_is_initialized();
    void juliaapi_check_initialized();
    void juliaapi_show(jl_value_t* t);
    void juliaapi_check_exception();

#else
    // load externally

    JL_EXTERN SEXP (*cast_xptr)(jl_value_t* s, bool preserve);
    JL_EXTERN jl_value_t* (*cast_jl_value_t)(SEXP s);

#endif

#ifdef JULIAAPI_INIT
    #include "juliaapi_init.h"
#endif

#endif
