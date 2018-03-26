#ifndef JULIAAPI_H
#define JULIAAPI_H

#define R_NO_REMAP
#include "R.h"
#include "Rinternals.h"

#include "julia.h"
#include "juliaapi_cast.h"

#if defined(JULIAAPI_INTERNAL) || !defined(JULIAAPI_INIT_H)

SEXP juliaapi_is_initialized();
void juliaapi_check_initialized();
void juliaapi_check_exception();
void juliaapi_show(SEXP s);

#else

SEXP (*juliaapi_is_initialized)();
void (*juliaapi_check_initialized)();
void (*juliaapi_check_exception)();
void (*juliaapi_show)(SEXP s);

#endif

#endif
