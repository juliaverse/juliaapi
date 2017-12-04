#include <Rcpp.h>
#include "../inst/include/juliaapi.h"
#include "xptr.h"

using namespace julia;
using namespace libjulia;
using namespace Rcpp;

// [[Rcpp::export]]
bool juliaapi_init(const std::string& libpath) {
    if (jl_main_module != NULL) {
        return true;
    }
    if (!load_libjulia(libpath)) {
        stop(libpath + " - " + get_last_dl_error_message());
    }
    if (!load_libjulia_symbols()) {
        stop(get_last_loaded_symbol() + " - " + get_last_dl_error_message());
    }

    jl_init();

    load_libjulia_constants();

    R_RegisterCCallable("juliaapi", "jl_eval_string", (DL_FUNC) jl_eval_string);

    return true;
}

// [[Rcpp::export]]
void juliaapi_load_constants(Environment env) {
    env["jl_any_type"] = cast_xptr(jl_any_type, false);

    env["jl_nothing"] = cast_xptr(jl_nothing, false);
    env["jl_true"] = cast_xptr(jl_true, false);
    env["jl_false"] = cast_xptr(jl_false, false);

    env["jl_main_module"] = cast_xptr(jl_main_module, false);
    env["jl_core_module"] = cast_xptr(jl_core_module, false);
    env["jl_base_module"] = cast_xptr(jl_base_module, false);
}

void juliaapi_check_exception() {
    jl_value_t* exception = jl_exception_occurred();
    if (exception) {
        // TODO: print to R STDERR
        jl_call2(jl_get_function(jl_base_module, "show"), jl_stderr_obj(), jl_exception_occurred());
        jl_printf(jl_stderr_stream(), "\n");
        stop(jl_typeof_str(exception));
    }
}

// [[Rcpp::export]]
void juliaapi_print(SEXP s) {
    // TODO: print to R STDOUT
    jl_call2(jl_get_function(jl_base_module, "show"), jl_stdout_obj(), cast_jl_value_t(s));
    jl_printf(jl_stderr_stream(), "\n");
    juliaapi_check_exception();
}


//' @export
// [[Rcpp::export(jl_eval_string)]]
SEXP juliaapi_eval_string(const char* str, bool preserve = true) {
    jl_value_t* result = jl_eval_string(str);
    juliaapi_check_exception();
    return cast_xptr(result, preserve);
}

//' @export
// [[Rcpp::export(jl_get_function)]]
SEXP juliaapi_get_function(SEXP module, const char* str) {
    jl_function_t* f = jl_get_function(cast_jl_value_t(module), str);
    if (f == nullptr) {
        stop("function not found in module.");
    }
    return cast_xptr(f, false);
}

//' @export
// [[Rcpp::export(jl_call)]]
SEXP juliaapi_call(SEXP f, List args) {
    int32_t nargs = args.size();
    jl_value_t** a = new jl_value_t*[nargs];
    for (int i = 0; i < nargs; i++) {
        a[i] = (jl_value_t*) cast_jl_value_t(args[i]);
    }
    jl_value_t* result = jl_call(cast_jl_value_t(f), a, nargs);
    delete[] a;
    juliaapi_check_exception();
    return cast_xptr(result);
}

//' @export
// [[Rcpp::export(jl_call0)]]
SEXP juliaapi_call0(SEXP f) {
    jl_value_t* result = jl_call0(cast_jl_value_t(f));
    juliaapi_check_exception();
    return cast_xptr(result);
}

//' @export
// [[Rcpp::export(jl_call1)]]
SEXP juliaapi_call1(SEXP f, SEXP a) {
    jl_value_t* result = jl_call1(cast_jl_value_t(f), cast_jl_value_t(a));
    juliaapi_check_exception();
    return cast_xptr(result);
}

//' @export
// [[Rcpp::export(jl_call2)]]
SEXP juliaapi_call2(SEXP f, SEXP a, SEXP b) {
    jl_value_t* result = jl_call2(
        cast_jl_value_t(f), cast_jl_value_t(a), cast_jl_value_t(b));
    juliaapi_check_exception();
    return cast_xptr(result);
}

//' @export
// [[Rcpp::export(jl_call3)]]
SEXP juliaapi_call3(SEXP f, SEXP a, SEXP b, SEXP c) {
    jl_value_t* result = jl_call3(
        cast_jl_value_t(f), cast_jl_value_t(a), cast_jl_value_t(b), cast_jl_value_t(c));
    juliaapi_check_exception();
    return cast_xptr(result);
}
