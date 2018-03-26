#include "../inst/include/juliaapi.h"

SEXP juliaapi_unbox_bool(SEXP v) {
    return Rf_ScalarLogical(jl_unbox_bool(cast_jl_value_t(v)));
}

SEXP juliaapi_unbox_int8(SEXP v) {
    return Rf_ScalarInteger(jl_unbox_int8(cast_jl_value_t(v)));
}

SEXP juliaapi_unbox_uint8(SEXP v) {
    return Rf_ScalarInteger(jl_unbox_uint8(cast_jl_value_t(v)));
}

SEXP juliaapi_unbox_int16(SEXP v) {
    return Rf_ScalarInteger(jl_unbox_int16(cast_jl_value_t(v)));
}

SEXP juliaapi_unbox_uint16(SEXP v) {
    return Rf_ScalarInteger(jl_unbox_uint16(cast_jl_value_t(v)));
}

SEXP juliaapi_unbox_int32(SEXP v) {
    return Rf_ScalarInteger(jl_unbox_int32(cast_jl_value_t(v)));
}

SEXP juliaapi_unbox_uint32(SEXP v) {
    return Rf_ScalarInteger(jl_unbox_uint32(cast_jl_value_t(v)));
}

SEXP juliaapi_unbox_int64(SEXP v) {
    return Rf_ScalarInteger(jl_unbox_int64(cast_jl_value_t(v)));
}

SEXP juliaapi_unbox_uint64(SEXP v) {
    return Rf_ScalarInteger(jl_unbox_uint64(cast_jl_value_t(v)));
}

SEXP juliaapi_unbox_float32(SEXP v) {
    return Rf_ScalarReal(jl_unbox_float32(cast_jl_value_t(v)));
}

SEXP juliaapi_unbox_float64(SEXP v) {
    return Rf_ScalarReal(jl_unbox_float64(cast_jl_value_t(v)));
}

SEXP juliaapi_unbox_voidpointer(SEXP v) {
    return R_MakeExternalPtr(jl_unbox_voidpointer(cast_jl_value_t(v)), R_NilValue, R_NilValue);
}
