#define JULIAAPI_INTERNAL
#include "../inst/include/juliaapi.h"
#include <Rcpp.h>

//' @export
// [[Rcpp::export(jl_is_initialized)]]
bool juliaapi_is_initialized() {
    return jl_main_module != nullptr;
}

void juliaapi_check_initialized() {
    if (!juliaapi_is_initialized()) {
        Rcpp::Environment juliaapienv("package:juliaapi");
        Rcpp::Function init = juliaapienv["jl_init"];
        init();
    }
}

//' @export
// [[Rcpp::export(jl_show)]]
void juliaapi_show(jl_value_t* t) {
    static jl_function_t* show = jl_get_function(jl_base_module, "show");
    static jl_function_t* sprint = jl_get_function(jl_base_module, "sprint");
    jl_value_t* ret = jl_call2(sprint, show, t);
    if (ret != nullptr) {
        char* s = jl_string_data(ret);
        Rprintf("%s\n", s);
    } else {
        juliaapi_check_exception();
    }
}

//' @export
// [[Rcpp::export(jl_check_exception)]]
void juliaapi_check_exception() {
    static jl_function_t* show = jl_get_function(jl_base_module, "showerror");
    static jl_function_t* sprint = jl_get_function(jl_base_module, "sprint");
    jl_value_t* exception = jl_exception_occurred();
    if (exception != nullptr) {
        jl_value_t* ret = jl_call2(sprint, show, exception);
        if (ret != nullptr) {
            char* s = jl_string_data(ret);
            Rcpp::stop("%s", s);
        } else {
            juliaapi_check_exception();
        }
    }
}
