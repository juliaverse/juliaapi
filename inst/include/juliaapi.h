#ifndef JULIAAPI_H
#define JULIAAPI_H

#define R_NO_REMAP
#include "R.h"
#include "Rinternals.h"

#include "julia.h"
#include "juliaapi_cast.h"

#ifdef JULIAAPI

// init
SEXP juliaapi_initialize(SEXP _libpath);
SEXP juliaapi_is_initialized();
SEXP juliaapi_check_initialized();
SEXP juliaapi_set_xptr(SEXP t, SEXP _name);
SEXP juliaapi_is_null_ptr(SEXP t);

// box
SEXP juliaapi_box_bool(SEXP x);
SEXP juliaapi_box_int8(SEXP x);
SEXP juliaapi_box_uint8(SEXP x);
SEXP juliaapi_box_int16(SEXP x);
SEXP juliaapi_box_uint16(SEXP x);
SEXP juliaapi_box_int32(SEXP x);
SEXP juliaapi_box_uint32(SEXP x);
SEXP juliaapi_box_char(SEXP x);
SEXP juliaapi_box_int64(SEXP x);
SEXP juliaapi_box_uint64(SEXP x);
SEXP juliaapi_box_float32(SEXP x);
SEXP juliaapi_box_float64(SEXP x);
SEXP juliaapi_box_voidpointer(SEXP x);

// unbox
SEXP juliaapi_unbox_bool(SEXP v);
SEXP juliaapi_unbox_int8(SEXP v);
SEXP juliaapi_unbox_uint8(SEXP v);
SEXP juliaapi_unbox_int16(SEXP v);
SEXP juliaapi_unbox_uint16(SEXP v);
SEXP juliaapi_unbox_int32(SEXP v);
SEXP juliaapi_unbox_uint32(SEXP v);
SEXP juliaapi_unbox_int64(SEXP v);
SEXP juliaapi_unbox_uint64(SEXP v);
SEXP juliaapi_unbox_float32(SEXP v);
SEXP juliaapi_unbox_float64(SEXP v);
SEXP juliaapi_unbox_voidpointer(SEXP v);

// eval
SEXP juliaapi_check_exception();
SEXP juliaapi_show(SEXP s);
SEXP juliaapi_eval_string(SEXP _str);
SEXP juliaapi_call0(SEXP f);
SEXP juliaapi_call1(SEXP f, SEXP a);
SEXP juliaapi_call2(SEXP f, SEXP a, SEXP b);
SEXP juliaapi_call3(SEXP f, SEXP a, SEXP b, SEXP c);

#else

// init
SEXP (*juliaapi_initialize)(SEXP _libpath);
SEXP (*juliaapi_is_initialized)();
SEXP (*juliaapi_check_initialized)();
SEXP (*juliaapi_set_xptr)(SEXP t, SEXP _name);
SEXP (*juliaapi_is_null_ptr)(SEXP t);

// box
SEXP (*juliaapi_box_bool)(SEXP x);
SEXP (*juliaapi_box_int8)(SEXP x);
SEXP (*juliaapi_box_uint8)(SEXP x);
SEXP (*juliaapi_box_int16)(SEXP x);
SEXP (*juliaapi_box_uint16)(SEXP x);
SEXP (*juliaapi_box_int32)(SEXP x);
SEXP (*juliaapi_box_uint32)(SEXP x);
SEXP (*juliaapi_box_char)(SEXP x);
SEXP (*juliaapi_box_int64)(SEXP x);
SEXP (*juliaapi_box_uint64)(SEXP x);
SEXP (*juliaapi_box_float32)(SEXP x);
SEXP (*juliaapi_box_float64)(SEXP x);
SEXP (*juliaapi_box_voidpointer)(SEXP x);

// unbox
SEXP (*juliaapi_unbox_bool)(SEXP v);
SEXP (*juliaapi_unbox_int8)(SEXP v);
SEXP (*juliaapi_unbox_uint8)(SEXP v);
SEXP (*juliaapi_unbox_int16)(SEXP v);
SEXP (*juliaapi_unbox_uint16)(SEXP v);
SEXP (*juliaapi_unbox_int32)(SEXP v);
SEXP (*juliaapi_unbox_uint32)(SEXP v);
SEXP (*juliaapi_unbox_int64)(SEXP v);
SEXP (*juliaapi_unbox_uint64)(SEXP v);
SEXP (*juliaapi_unbox_float32)(SEXP v);
SEXP (*juliaapi_unbox_float64)(SEXP v);
SEXP (*juliaapi_unbox_voidpointer)(SEXP v);

// eval
SEXP (*juliaapi_check_exception)();
SEXP (*juliaapi_show)(SEXP s);
SEXP (*juliaapi_eval_string)(SEXP _str);
SEXP (*juliaapi_call0)(SEXP f);
SEXP (*juliaapi_call1)(SEXP f, SEXP a);
SEXP (*juliaapi_call2)(SEXP f, SEXP a, SEXP b);
SEXP (*juliaapi_call3)(SEXP f, SEXP a, SEXP b, SEXP c);

#endif

#endif
