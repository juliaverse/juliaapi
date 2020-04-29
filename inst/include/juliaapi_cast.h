#ifndef JULIAAPI_CAST_H
#define JULIAAPI_CAST_H

#ifdef JULIAAPI

SEXP cast_xptr(jl_value_t* s, int preserve);
jl_value_t* cast_jl_value_t(SEXP s);

#else

SEXP (*cast_xptr)(jl_value_t* s, int preserve);
jl_value_t* (*cast_jl_value_t)(SEXP s);

#endif

#endif
