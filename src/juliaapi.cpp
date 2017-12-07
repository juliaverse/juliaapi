#define JULIAAPI_CPP
#define JULIAAPI_INTERNAL
#define JULIAAPI_INIT
#include "../inst/include/juliaapi.h"
#include <Rcpp.h>

using namespace Rcpp;

// [[Rcpp::export]]
bool juliaapi_is_initialized() {
    if (jl_main_module == nullptr) {
        return false;
    }
    return jl_is_initialized();
}

#define REGISTER_JULIAAPI_SYMBOL(name) \
    R_RegisterCCallable("juliaapi", #name, (DL_FUNC) name);

// [[Rcpp::export]]
bool juliaapi_init(const std::string& libpath) {
    if (jl_main_module != nullptr) {
        return true;
    }
    if (!load_libjulia(libpath)) {
        stop(libpath + " - " + get_last_dl_error_message());
    }
    if (!load_libjulia_symbols()) {
        stop(get_last_loaded_symbol() + " - " + get_last_dl_error_message());
    }

    jl_init();

    if (!load_libjulia_constants()) {
        stop(get_last_loaded_symbol() + " - " + get_last_dl_error_message());
    }
    REGISTER_JULIAAPI_SYMBOL(load_libjulia_symbol);
    REGISTER_JULIAAPI_SYMBOL(load_libjulia_constant);
    REGISTER_JULIAAPI_SYMBOL(cast_xptr);
    REGISTER_JULIAAPI_SYMBOL(cast_jl_value_t);

    REGISTER_JULIAAPI_SYMBOL(juliaapi_check_exception);
    REGISTER_JULIAAPI_SYMBOL(juliaapi_print);
    REGISTER_JULIAAPI_SYMBOL(juliaapi_eval_string);

    return true;
}

// [[Rcpp::export]]
bool juliaapi_is_null_xptr(SEXP t) {
    if (TYPEOF(t) != EXTPTRSXP) {
        stop("exptect an externalptr");
    }
    return R_ExternalPtrAddr(t) == nullptr;
}

// [[Rcpp::export]]
void juliaapi_set_xptr(SEXP t, const std::string& name) {
    void* p;
    load_libjulia_constant(name, (void**) &p);
    R_SetExternalPtrAddr(t, p);
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


// [[Rcpp::export]]
SEXP juliaapi_eval_string(const char* str, bool preserve = true) {
    jl_value_t* result = jl_eval_string(str);
    juliaapi_check_exception();
    return cast_xptr(result, preserve);
}

// [[Rcpp::export]]
SEXP juliaapi_get_function(jl_value_t* module, const char* str) {
    jl_function_t* f = jl_get_function((jl_module_t*) module, str);
    if (f == nullptr) {
        stop("function not found in module.");
    }
    return cast_xptr(f, false);
}

// [[Rcpp::export]]
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

// [[Rcpp::export]]
jl_value_t* juliaapi_call0(jl_value_t* f) {
    jl_value_t* result = jl_call0(f);
    juliaapi_check_exception();
    return result;
}

// [[Rcpp::export]]
jl_value_t* juliaapi_call1(jl_value_t* f, jl_value_t* a) {
    jl_value_t* result = jl_call1(f, a);
    juliaapi_check_exception();
    return result;
}

// [[Rcpp::export]]
jl_value_t* juliaapi_call2(jl_value_t* f, jl_value_t* a, jl_value_t* b) {
    jl_value_t* result = jl_call2(f, a, b);
    juliaapi_check_exception();
    return result;
}

// [[Rcpp::export]]
jl_value_t* juliaapi_call3(jl_value_t* f, jl_value_t* a, jl_value_t* b, jl_value_t* c) {
    jl_value_t* result = jl_call3(f, a, b, c);
    juliaapi_check_exception();
    return result;
}
