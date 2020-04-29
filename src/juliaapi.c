#define JULIAAPI
#include "../inst/include/juliaapi.h"

static const R_CallMethodDef CallEntries[] = {
    // init
    {"juliaapi_initialize", (DL_FUNC) &juliaapi_initialize, 1},
    {"juliaapi_is_initialized", (DL_FUNC) &juliaapi_is_initialized, 0},
    {"juliaapi_check_initialized", (DL_FUNC) &juliaapi_check_initialized, 0},
    {"juliaapi_set_xptr", (DL_FUNC) &juliaapi_set_xptr, 2},
    {"juliaapi_is_null_ptr", (DL_FUNC) &juliaapi_is_null_ptr, 1},

    // box
    {"juliaapi_box_bool", (DL_FUNC) &juliaapi_box_bool, 1},
    {"juliaapi_box_int8", (DL_FUNC) &juliaapi_box_int8, 1},
    {"juliaapi_box_uint8", (DL_FUNC) &juliaapi_box_uint8, 1},
    {"juliaapi_box_int16", (DL_FUNC) &juliaapi_box_int16, 1},
    {"juliaapi_box_uint16", (DL_FUNC) &juliaapi_box_uint16, 1},
    {"juliaapi_box_int32", (DL_FUNC) &juliaapi_box_int32, 1},
    {"juliaapi_box_uint32", (DL_FUNC) &juliaapi_box_uint32, 1},
    {"juliaapi_box_char", (DL_FUNC) &juliaapi_box_char, 1},
    {"juliaapi_box_int64", (DL_FUNC) &juliaapi_box_int64, 1},
    {"juliaapi_box_uint64", (DL_FUNC) &juliaapi_box_uint64, 1},
    {"juliaapi_box_float32", (DL_FUNC) &juliaapi_box_float32, 1},
    {"juliaapi_box_float64", (DL_FUNC) &juliaapi_box_float64, 1},
    {"juliaapi_box_voidpointer", (DL_FUNC) &juliaapi_box_voidpointer, 1},
    // unbox
    {"juliaapi_unbox_bool", (DL_FUNC) &juliaapi_unbox_bool, 1},
    {"juliaapi_unbox_int8", (DL_FUNC) &juliaapi_unbox_int8, 1},
    {"juliaapi_unbox_uint8", (DL_FUNC) &juliaapi_unbox_uint8, 1},
    {"juliaapi_unbox_int16", (DL_FUNC) &juliaapi_unbox_int16, 1},
    {"juliaapi_unbox_uint16", (DL_FUNC) &juliaapi_unbox_uint16, 1},
    {"juliaapi_unbox_int32", (DL_FUNC) &juliaapi_unbox_int32, 1},
    {"juliaapi_unbox_uint32", (DL_FUNC) &juliaapi_unbox_uint32, 1},
    {"juliaapi_unbox_int64", (DL_FUNC) &juliaapi_unbox_int64, 1},
    {"juliaapi_unbox_uint64", (DL_FUNC) &juliaapi_unbox_uint64, 1},
    {"juliaapi_unbox_float32", (DL_FUNC) &juliaapi_unbox_float32, 1},
    {"juliaapi_unbox_float64", (DL_FUNC) &juliaapi_unbox_float64, 1},
    {"juliaapi_unbox_voidpointer", (DL_FUNC) &juliaapi_unbox_voidpointer, 1},
    // eval
    {"juliaapi_check_exception", (DL_FUNC) &juliaapi_check_exception, 0},
    {"juliaapi_show", (DL_FUNC) &juliaapi_show, 1},
    {"juliaapi_eval_string", (DL_FUNC) &juliaapi_eval_string, 1},
    {"juliaapi_call0", (DL_FUNC) &juliaapi_call0, 1},
    {"juliaapi_call1", (DL_FUNC) &juliaapi_call1, 2},
    {"juliaapi_call2", (DL_FUNC) &juliaapi_call2, 3},
    {"juliaapi_call3", (DL_FUNC) &juliaapi_call3, 4},

    {NULL, NULL, 0}
};

void R_init_juliaapi(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
