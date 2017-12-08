#define JULIAAPI_INTERNAL
#include "../inst/include/juliaapi.h"
#include <Rcpp.h>

//' @export
// [[Rcpp::export(jl_eval_string)]]
jl_value_t* juliaapi_eval_string(const char* str) {
    juliaapi_check_initialized();
    jl_value_t* result = jl_eval_string(str);
    juliaapi_check_exception();
    return result;
}

// [[Rcpp::export(jl_exception_occurred)]]
jl_value_t* julia_exception_occurred() {
    return jl_exception_occurred();
}

//' @export
// [[Rcpp::export(jl_get_function)]]
SEXP juliaapi_get_function(jl_value_t* module, const char* str) {
    juliaapi_check_initialized();
    jl_function_t* f = jl_get_function((jl_module_t*) module, str);
    juliaapi_check_exception();
    return cast_xptr(f, false);
}

//' @export
jl_value_t* juliaapi_call(jl_value_t* f, Rcpp::List args) {
    int nargs = args.size();
    juliaapi_check_initialized();
    jl_value_t** a = new jl_value_t*[nargs];
    for (int i = 0; i < nargs; i++) {
        a[i] = (jl_value_t*) cast_jl_value_t(args[i]);
    }
    jl_value_t* result = jl_call(f, a, nargs);
    delete[] a;
    juliaapi_check_exception();
    return result;
}

//' @export
// [[Rcpp::export(jl_call0)]]
jl_value_t* juliaapi_call0(jl_value_t* f) {
    juliaapi_check_initialized();
    jl_value_t* result = jl_call0(f);
    juliaapi_check_exception();
    return result;
}

//' @export
// [[Rcpp::export(jl_call1)]]
jl_value_t* juliaapi_call1(jl_value_t* f, jl_value_t* a) {
    juliaapi_check_initialized();
    jl_value_t* result = jl_call1(f, a);
    juliaapi_check_exception();
    return result;
}

//' @export
// [[Rcpp::export(jl_call2)]]
jl_value_t* jualiapi_call2(jl_value_t* f, jl_value_t* a, jl_value_t* b) {
    juliaapi_check_initialized();
    jl_value_t* result = jl_call2(f, a, b);
    juliaapi_check_exception();
    return result;
}

//' @export
// [[Rcpp::export(jl_call3)]]
jl_value_t* jualiapi_call3(jl_value_t* f, jl_value_t* a, jl_value_t* b, jl_value_t* c) {
    juliaapi_check_initialized();
    jl_value_t* result = jl_call3(f, a, b, c);
    juliaapi_check_exception();
    return result;
}

//' @export
// [[Rcpp::export(jl_isa)]]
int juliaapi_isa(jl_value_t* a, jl_value_t* t) {
    return jl_isa(a, t);
}

//' @export
// [[Rcpp::export(jl_types_equal)]]
int juliaapi_types_equal(jl_value_t* a, jl_value_t* b) {
    return jl_types_equal(a, b);
}

//' @export
// [[Rcpp::export(jl_typename_str)]]
const char* juliaapi_typename_str(jl_value_t* v) {
    return jl_typename_str(v);
}

//' @export
// [[Rcpp::export(jl_typeof_str)]]
const char* juliaapi_typeof_str(jl_value_t* v) {
    return jl_typeof_str(v);
}

//' @export
// [[Rcpp::export(jl_symbol)]]
jl_value_t* juliaapi_symbol(const char* str) {
    return (jl_value_t*) jl_symbol(str);
}
