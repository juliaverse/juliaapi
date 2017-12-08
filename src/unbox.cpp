#define JULIAAPI_INTERNAL
#define JULIAAPI_CPP
#include "../inst/include/juliaapi.h"
#include <Rcpp.h>

using namespace Rcpp;

//' @export
// [[Rcpp::export(jl_unbox_bool)]]
bool juliaapi_unbox_bool(jl_value_t* v) {
    return (int) jl_unbox_bool(v);
}

//' @export
// [[Rcpp::export(jl_unbox_int8)]]
int juliaapi_unbox_int8(jl_value_t* v) {
    return (int) jl_unbox_int8(v);
}

//' @export
// [[Rcpp::export(jl_unbox_uint8)]]
int juliaapi_unbox_uint8(jl_value_t* v) {
    return (int) jl_unbox_uint8(v);
}

//' @export
// [[Rcpp::export(jl_unbox_int16)]]
int juliaapi_unbox_int16(jl_value_t* v) {
    return (int) jl_unbox_int16(v);
}

//' @export
// [[Rcpp::export(jl_unbox_uint16)]]
int juliaapi_unbox_uint16(jl_value_t* v) {
    return (int) jl_unbox_uint16(v);
}

//' @export
// [[Rcpp::export(jl_unbox_int32)]]
int juliaapi_unbox_int32(jl_value_t* v) {
    return (int) jl_unbox_int32(v);
}

//' @export
// [[Rcpp::export(jl_unbox_uint32)]]
int juliaapi_unbox_uint32(jl_value_t* v) {
    return (int) jl_unbox_uint32(v);
}

//' @export
// [[Rcpp::export(jl_unbox_int64)]]
int juliaapi_unbox_int64(jl_value_t* v) {
    return (int) jl_unbox_int64(v);
}

//' @export
// [[Rcpp::export(jl_unbox_uint64)]]
int juliaapi_unbox_uint64(jl_value_t* v) {
    return (int) jl_unbox_uint64(v);
}

//' @export
// [[Rcpp::export(jl_unbox_float32)]]
double juliaapi_unbox_float32(jl_value_t* v) {
    return (double) jl_unbox_float32(v);
}

//' @export
// [[Rcpp::export(jl_unbox_float64)]]
double juliaapi_unbox_float64(jl_value_t* v) {
    return (double) jl_unbox_float64(v);
}

//' @export
// [[Rcpp::export(jl_unbox_voidpointer)]]
SEXP juliaapi_unbox_voidpointer(jl_value_t* v) {
    return R_MakeExternalPtr(jl_unbox_voidpointer(v), R_NilValue, R_NilValue);
}
