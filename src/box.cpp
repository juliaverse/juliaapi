#define JULIAAPI_INTERNAL
#include "../inst/include/juliaapi.h"
#include <Rcpp.h>

using namespace Rcpp;

//' @export
// [[Rcpp::export(jl_box_bool)]]
jl_value_t* juliaapi_box_bool(int x) {
    return jl_box_bool((int8_t) x);
}

//' @export
// [[Rcpp::export(jl_box_int8)]]
jl_value_t* juliaapi_box_int8(int x) {
    return jl_box_int8((int8_t) x);
}

//' @export
// [[Rcpp::export(jl_box_uint8)]]
jl_value_t* juliaapi_box_uint8(int x) {
    return jl_box_uint8((uint8_t) x);
}

//' @export
// [[Rcpp::export(jl_box_int16)]]
jl_value_t* juliaapi_box_int16(int x) {
    return jl_box_int16((int16_t) x);
}

//' @export
// [[Rcpp::export(jl_box_uint16)]]
jl_value_t* juliaapi_box_uint16(int x) {
    return jl_box_uint16((uint16_t) x);
}

//' @export
// [[Rcpp::export(jl_box_int32)]]
jl_value_t* juliaapi_box_int32(int x) {
    return jl_box_int32((int32_t) x);
}

//' @export
// [[Rcpp::export(jl_box_uint32)]]
jl_value_t* juliaapi_box_uint32(int x) {
    return jl_box_uint32((uint32_t) x);
}

//' @export
// [[Rcpp::export(jl_box_char)]]
jl_value_t* juliaapi_box_char(int x) {
    return jl_box_char((uint32_t) x);
}

//' @export
// [[Rcpp::export(jl_box_int64)]]
jl_value_t* juliaapi_box_int64(int x) {
    return jl_box_int64((int64_t) x);
}

//' @export
// [[Rcpp::export(jl_box_uint64)]]
jl_value_t* juliaapi_box_uint64(int x) {
    return jl_box_uint64((uint64_t) x);
}

//' @export
// [[Rcpp::export(jl_box_float32)]]
jl_value_t* juliaapi_box_float32(double x) {
    return jl_box_float32((float) x);
}

//' @export
// [[Rcpp::export(jl_box_float64)]]
jl_value_t* juliaapi_box_float64(double x) {
    return jl_box_float64((double) x);
}

//' @export
// [[Rcpp::export(jl_box_voidpointer)]]
jl_value_t* juliaapi_box_voidpointer(SEXP x) {
    if (TYPEOF(x) != EXTPTRSXP) {
        Rcpp::stop("exptect an externalptr");
    }
    return jl_box_voidpointer(R_ExternalPtrAddr(x));
}
