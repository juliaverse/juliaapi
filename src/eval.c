#define JULIAAPI
#include "../inst/include/juliaapi.h"

void juliaapi_check_exception() {
    jl_function_t* show = jl_get_function(jl_base_module, "showerror");
    jl_function_t* sprint = jl_get_function(jl_base_module, "sprint");
    jl_value_t* exception = jl_exception_occurred();
    if (exception != NULL) {
        jl_value_t* ret = jl_call2(sprint, show, exception);
        if (ret != NULL) {
            Rf_error("%s", jl_string_data(ret));
        } else {
            juliaapi_check_exception();
        }
    }
}

void juliaapi_show(SEXP s) {
    jl_value_t* t = (jl_value_t*) R_ExternalPtrAddr(s);
    jl_function_t* show = jl_get_function(jl_base_module, "show");
    jl_value_t* mime = jl_eval_string("MIME(\"text/plain\")");
    jl_function_t* sprint = jl_get_function(jl_base_module, "sprint");
    jl_value_t* ret = jl_call3(sprint, show, mime, t);
    if (ret != NULL) {
        Rprintf("%s\n", jl_string_data(ret));
    } else {
        juliaapi_check_exception();
    }
}

SEXP juliaapi_eval_string(SEXP _str) {
    const char* str = R_CHAR(Rf_asChar(_str));
    jl_value_t* result = jl_eval_string(str);
    juliaapi_check_exception();
    return cast_xptr(result, 1);
}

SEXP juliaapi_call0(SEXP f) {
    juliaapi_check_initialized();
    jl_value_t* result = jl_call0(cast_jl_value_t(f));
    juliaapi_check_exception();
    return cast_xptr(result, 1);
}

SEXP juliaapi_call1(SEXP f, SEXP a) {
    juliaapi_check_initialized();
    jl_value_t* result = jl_call1(cast_jl_value_t(f), cast_jl_value_t(a));
    juliaapi_check_exception();
    return cast_xptr(result, 1);
}

SEXP jualiapi_call2(SEXP f, SEXP a, SEXP b) {
    juliaapi_check_initialized();
    jl_value_t* result = jl_call2(cast_jl_value_t(f), cast_jl_value_t(a), cast_jl_value_t(b));
    juliaapi_check_exception();
    return cast_xptr(result, 1);
}

SEXP jualiapi_call3(SEXP f, SEXP a, SEXP b, SEXP c) {
    juliaapi_check_initialized();
    jl_value_t* result = jl_call3(
        cast_jl_value_t(f), cast_jl_value_t(a), cast_jl_value_t(b), cast_jl_value_t(c));
    juliaapi_check_exception();
    return cast_xptr(result, 1);
}
