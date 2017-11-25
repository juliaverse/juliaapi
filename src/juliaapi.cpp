#include <Rcpp.h>
#include "libjulia.h"

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

    if (!load_libjulia_modules()) {
        stop(get_last_dl_error_message());
    }

    return true;
}


// [[Rcpp::export]]
SEXP juliaapi_eval_string(const char* str, bool preserve = false) {
    jl_value_t* result = jl_eval_string(str);
    jl_value_t* exception = jl_exception_occurred();
    if (exception) {
        jl_call2(jl_get_function(jl_base_module, "show"), jl_stderr_obj(), jl_exception_occurred());
        jl_printf(jl_stderr_stream(), "\n");
        stop(jl_typeof_str(exception));
    }
    if (preserve) {
        // default
    }
    return XPtr<jl_value_t>((jl_value_t*) result);
}
