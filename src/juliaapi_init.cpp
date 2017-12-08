#define JULIAAPI_INTERNAL
#define JULIAAPI_INIT
#include "../inst/include/juliaapi.h"
#include <Rcpp.h>

using namespace Rcpp;

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

    return true;
}

// [[Rcpp::export]]
bool jl_is_null_xptr(SEXP t) {
    if (TYPEOF(t) != EXTPTRSXP) {
        Rcpp::stop("exptect an externalptr");
    }
    return R_ExternalPtrAddr(t) == nullptr;
}

// [[Rcpp::export]]
void jl_set_xptr(SEXP t, const std::string& name) {
    void* p;
    load_libjulia_constant(name, (void**) &p);
    R_SetExternalPtrAddr(t, p);
}
