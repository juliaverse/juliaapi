#ifndef JULIAAPI_H
#define JULIAAPI_H

#ifdef JULIAAPI_LOADING
    #include "libjulia.h"
#endif

#include "julia.h"

#include <RcppCommon.h>

namespace Rcpp {
    SEXP wrap(jl_value_t* object);
    template<> jl_value_t* as(SEXP object);
}


#ifdef JULIAAPI_LOADING
#ifdef JULIAAPI_CPP
// load internally

SEXP cast_xptr(jl_value_t* s, bool preserve);
jl_value_t* cast_jl_value_t(SEXP s);

void juliaapi_init();

void juliaapi_check_exception();
void juliaapi_print(jl_value_t* t);
SEXP juliaapi_eval_string(const char* str, bool preserve);

#else
// load externally

SEXP (*cast_xptr)(jl_value_t* s, bool preserve);
jl_value_t* (*cast_jl_value_t)(SEXP s);

void juliaapi_init() {
    libjulia::load_symbol("cast_xptr", (void**) &cast_xptr);
    libjulia::load_symbol("cast_jl_value_t", (void**) &cast_jl_value_t);

    libjulia::load_symbol("juliaapi_check_exception", (void**) &juliaapi_check_exception);
    libjulia::load_symbol("juliaapi_print", (void**) &juliaapi_print);
    libjulia::load_symbol("juliaapi_eval_string", (void**) &juliaapi_eval_string);

    libjulia::load_symbols();
    libjulia::load_constants();
}

void (*juliaapi_check_exception)();
void (*juliaapi_print)(jl_value_t* t);
SEXP (*juliaapi_eval_string)(const char* str, bool preserve);

#endif

namespace Rcpp {

    SEXP wrap(jl_value_t* object) {
        return cast_xptr(object, true);
    }

    template<> jl_value_t* as(SEXP object) {
        return cast_jl_value_t(object);
    }

}

#endif // load externally

#endif
