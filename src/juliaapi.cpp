#define JULIAAPI_CPP
#define JULIAAPI_INIT
#include "../inst/include/juliaapi.h"
#include <Rcpp.h>

using namespace Rcpp;

// [[Rcpp::export]]
bool juliaapi_init(const std::string& libpath) {
    if (jl_main_module != NULL) {
        return true;
    }
    if (!load_julia(libpath)) {
        stop(libpath + " - " + get_last_dl_error_message());
    }
    if (!load_julia_symbols()) {
        stop(get_last_loaded_symbol() + " - " + get_last_dl_error_message());
    }

    jl_init();

    load_julia_constants();

    R_RegisterCCallable("juliaapi", "cast_xptr", (DL_FUNC) cast_xptr);
    R_RegisterCCallable("juliaapi", "cast_jl_value_t", (DL_FUNC) cast_jl_value_t);

    R_RegisterCCallable("juliaapi", "juliaapi_check_exception", (DL_FUNC) juliaapi_check_exception);
    R_RegisterCCallable("juliaapi", "juliaapi_print", (DL_FUNC) juliaapi_print);
    R_RegisterCCallable("juliaapi", "juliaapi_eval_string", (DL_FUNC) juliaapi_eval_string);

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
void juliaapi_print(jl_value_t* t) {
    // TODO: print to R STDOUT
    jl_call2(jl_get_function(jl_base_module, "show"), jl_stdout_obj(), t);
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
SEXP juliaapi_get_function(jl_value_t* module, const char* str) {
    jl_function_t* f = jl_get_function(module, str);
    if (f == nullptr) {
        stop("function not found in module.");
    }
    return cast_xptr(f, false);
}

//' @export
// [[Rcpp::export(jl_call)]]
jl_value_t* juliaapi_call(jl_value_t* f, List args) {
    int32_t nargs = args.size();
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
    jl_value_t* result = jl_call0(f);
    juliaapi_check_exception();
    return result;
}

//' @export
// [[Rcpp::export(jl_call1)]]
jl_value_t* juliaapi_call1(jl_value_t* f, jl_value_t* a) {
    jl_value_t* result = jl_call1(f, a);
    juliaapi_check_exception();
    return result;
}

//' @export
// [[Rcpp::export(jl_call2)]]
jl_value_t* juliaapi_call2(jl_value_t* f, jl_value_t* a, jl_value_t* b) {
    jl_value_t* result = jl_call2(f, a, b);
    juliaapi_check_exception();
    return result;
}

//' @export
// [[Rcpp::export(jl_call3)]]
jl_value_t* juliaapi_call3(jl_value_t* f, jl_value_t* a, jl_value_t* b, jl_value_t* c) {
    jl_value_t* result = jl_call3(f, a, b, c);
    juliaapi_check_exception();
    return result;
}
