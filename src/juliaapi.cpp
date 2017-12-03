#include <Rcpp.h>
#include <stack>
#include "libjulia.h"

using namespace libjulia;
using namespace Rcpp;


std::map<jl_value_t*, std::pair<size_t, size_t> > gc_index_map;
std::stack<size_t> gc_free_stack;

jl_array_t* get_gc_array() {
    static jl_module_t* m = nullptr;
    if (m == nullptr) {
        m = (jl_module_t*) jl_new_module(jl_symbol("#JuliaAPI"));
        jl_set_const(jl_main_module, jl_symbol("#JuliaAPI"), (jl_value_t*) m);
    }

    static jl_array_t* arr = nullptr;
    if (arr == nullptr) {
        jl_value_t* array_type = jl_apply_array_type(jl_any_type, 1);
        arr = jl_alloc_array_1d(array_type, 0);
        jl_set_const(m, jl_symbol("gc_array"), (jl_value_t*) arr);
    }
    return arr;
}

void juliaapi_gc_preserve(jl_value_t* s) {
    JL_GC_PUSH1(&s);

    // if it is already preserved, increase reference count and exit
    std::map<jl_value_t*, std::pair<size_t, size_t> >::iterator it;
    it = gc_index_map.find(s);
    if(it != gc_index_map.end()) {
        it->second.second += 1;
        JL_GC_POP();
        return;
    }

    jl_array_t* arr = get_gc_array();
    size_t pos = 0;
    if (gc_free_stack.empty()) {
        pos = jl_array_len(arr);
        jl_array_grow_end(arr, 1);
    } else {
        pos = gc_free_stack.top();
        gc_free_stack.pop();
    }
    // put the julia object to the array to protect it from gc
    jl_arrayset(arr, s, pos);
    gc_index_map[s] = std::make_pair(pos, 1);;
    JL_GC_POP();
}

void juliaapi_gc_release(jl_value_t* s) {
    std::map<jl_value_t*, std::pair<size_t, size_t> >::iterator it;
    it = gc_index_map.find(s);
    if(it == gc_index_map.end()) {
        stop("trying to release an unprotected julia object.");
    } else {
        it->second.second -= 1;
        if(it->second.second == 0) {
            gc_free_stack.push(it->second.first);
            jl_arrayset(get_gc_array(), jl_nothing, it->second.first);
            gc_index_map.erase(it);
        }
    }
}

void xptr_finalizer(SEXP s) {
    juliaapi_gc_release((jl_value_t*) R_ExternalPtrAddr(s));
}

SEXP cast_xptr(jl_value_t* s, bool preserve = true) {
    SEXP sp;
    if (preserve) {
        juliaapi_gc_preserve(s);
    }
    sp = PROTECT(R_MakeExternalPtr((void*) s, R_NilValue, R_NilValue));
    R_RegisterCFinalizerEx(sp, &xptr_finalizer, (Rboolean) 0);
    Rf_setAttrib(sp, R_ClassSymbol, Rf_mkString("jl_value"));
    UNPROTECT(1);
    return sp;
}

jl_value_t* cast_jl_value_t(SEXP s) {
    return (jl_value_t*) R_ExternalPtrAddr(s);
}

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
SEXP _jl_eval_string(const char* str, bool preserve = true) {
    jl_value_t* result = jl_eval_string(str);
    juliaapi_check_exception();
    return cast_xptr(result, preserve);
}

//' @export
// [[Rcpp::export(jl_get_function)]]
SEXP _jl_get_function(SEXP module, const char* str) {
    jl_function_t* f = jl_get_function(cast_jl_value_t(module), str);
    if (f == nullptr) {
        stop("function not found in module.");
    }
    return cast_xptr(f, false);
}

//' @export
// [[Rcpp::export(jl_call)]]
SEXP _jl_call(SEXP f, List args) {
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
SEXP _jl_call0(SEXP f) {
    jl_value_t* result = jl_call0(cast_jl_value_t(f));
    juliaapi_check_exception();
    return cast_xptr(result);
}

//' @export
// [[Rcpp::export(jl_call1)]]
SEXP _jl_call1(SEXP f, SEXP a) {
    jl_value_t* result = jl_call1(cast_jl_value_t(f), cast_jl_value_t(a));
    juliaapi_check_exception();
    return cast_xptr(result);
}

//' @export
// [[Rcpp::export(jl_call2)]]
SEXP _jl_call2(SEXP f, SEXP a, SEXP b) {
    jl_value_t* result = jl_call2(
        cast_jl_value_t(f), cast_jl_value_t(a), cast_jl_value_t(b));
    juliaapi_check_exception();
    return cast_xptr(result);
}

//' @export
// [[Rcpp::export(jl_call3)]]
SEXP _jl_call3(SEXP f, SEXP a, SEXP b, SEXP c) {
    jl_value_t* result = jl_call3(
        cast_jl_value_t(f), cast_jl_value_t(a), cast_jl_value_t(b), cast_jl_value_t(c));
    juliaapi_check_exception();
    return cast_xptr(result);
}
