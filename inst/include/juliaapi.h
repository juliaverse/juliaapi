#ifndef JULIAAPI_H
#define JULIAAPI_H

#define R_NO_REMAP
#include "R.h"
#include "Rinternals.h"

#include "julia.h"
#include "juliaapi_cast.h"

#if defined(JULIAAPI_INTERNAL) || !defined(JULIAAPI_INIT_H)

void juliaapi_check_exception();
void juliaapi_show(SEXP s);
SEXP cast_xptr(jl_value_t* s, int preserve);
jl_value_t* cast_jl_value_t(SEXP s);

#else

void (*juliaapi_check_exception)();
void (*juliaapi_show)(SEXP s);
SEXP (*cast_xptr)(jl_value_t* s, int preserve);
jl_value_t* (*cast_jl_value_t)(SEXP s);

#endif

#endif
