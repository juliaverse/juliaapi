#define JULIAAPI
#include "../inst/include/juliaapi_init.h"


SEXP juliaapi_initialize(SEXP _libpath) {
    const char* libpath = R_CHAR(Rf_asChar(_libpath));
    if (!load_julia(libpath)) {
        Rf_error("%s", get_dl_error_message());
    }
    if (!juliaapi_init()) {
        Rf_error("%s -- %s", get_dl_error_message(), get_last_loaded_symbol);
    }
    return Rf_ScalarLogical(1);
}

SEXP juliaapi_is_initialized() {
    return Rf_ScalarLogical(jl_main_module != NULL);
}

SEXP juliaapi_check_initialized() {
    return R_NilValue;
}

SEXP juliaapi_set_xptr(SEXP t, SEXP _name) {
    void* p;
    const char* name = R_CHAR(Rf_asChar(_name));
    load_julia_constant(name, (void**) &p);
    R_SetExternalPtrAddr(t, p);
    return t;
}

SEXP juliaapi_is_null_ptr(SEXP t) {
    if (TYPEOF(t) != EXTPTRSXP) {
        Rf_error("expect an externalptr");
    }
    return Rf_ScalarLogical(R_ExternalPtrAddr(t) == NULL);
}
