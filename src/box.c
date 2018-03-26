#include "../inst/include/juliaapi.h"

SEXP juliaapi_box_bool(SEXP x) {
    return cast_xptr(jl_box_bool((int8_t) Rf_asInteger(x)), 0);
}

SEXP juliaapi_box_int8(SEXP x) {
    return cast_xptr(jl_box_int8((int8_t) Rf_asInteger(x)), 0);
}

SEXP juliaapi_box_uint8(SEXP x) {
    return cast_xptr(jl_box_uint8((uint8_t) Rf_asInteger(x)), 0);
}

SEXP juliaapi_box_int16(SEXP x) {
    return cast_xptr(jl_box_int16((int16_t) Rf_asInteger(x)), 0);
}

SEXP juliaapi_box_uint16(SEXP x) {
    return cast_xptr(jl_box_uint16((uint16_t) Rf_asInteger(x)), 0);
}

SEXP juliaapi_box_int32(SEXP x) {
    return cast_xptr(jl_box_int32((int32_t) Rf_asInteger(x)), 0);
}

SEXP juliaapi_box_uint32(SEXP x) {
    return cast_xptr(jl_box_uint32((uint32_t) Rf_asInteger(x)), 0);
}

SEXP juliaapi_box_char(SEXP x) {
    const char* c = R_CHAR(Rf_asChar(x));
    if (strlen(c) == 0) {
        Rf_error("expect non empty char");
    }
    return cast_xptr(jl_box_char((uint32_t) c[0]), 0);
}

SEXP juliaapi_box_int64(SEXP x) {
    return cast_xptr(jl_box_int64((int64_t) Rf_asInteger(x)), 0);
}

SEXP juliaapi_box_uint64(SEXP x) {
    return cast_xptr(jl_box_uint64((uint64_t) Rf_asInteger(x)), 0);
}

SEXP juliaapi_box_float32(SEXP x) {
    return cast_xptr(jl_box_float32((float) Rf_asReal(x)), 0);
}

SEXP juliaapi_box_float64(SEXP x) {
    return cast_xptr(jl_box_float64((double) Rf_asReal(x)), 0);
}

SEXP juliaapi_box_voidpointer(SEXP x) {
    if (TYPEOF(x) != EXTPTRSXP) {
        Rf_error("exptect an externalptr");
    }
    return cast_xptr((jl_value_t*) jl_box_voidpointer(R_ExternalPtrAddr(x)), 1);
}
