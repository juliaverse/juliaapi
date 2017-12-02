#include <stack>
#include <Rcpp.h>
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

typedef XPtr<jl_value_t, NoProtectStorage, juliaapi_gc_release> jl_value_xptr;

SEXP juliaapi_make_xptr(jl_value_t* s, bool preserve = true) {
    if (preserve) {
        juliaapi_gc_preserve(s);
        return jl_value_xptr(s);
    } else {
        return jl_value_xptr(s, false);
    }
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

void juliaapi_check_exception() {
    jl_value_t* exception = jl_exception_occurred();
    if (exception) {
        jl_call2(jl_get_function(jl_base_module, "show"), jl_stderr_obj(), jl_exception_occurred());
        jl_printf(jl_stderr_stream(), "\n");
        stop(jl_typeof_str(exception));
    }
}


// [[Rcpp::export]]
SEXP juliaapi_eval_string(const char* str, bool preserve = true) {
    jl_value_t* result = jl_eval_string(str);
    juliaapi_check_exception();
    return juliaapi_make_xptr(result, preserve);
}

// [[Rcpp::export]]
void juliaapi_print(SEXP s) {
    jl_value_xptr p(s);
    jl_call2(jl_get_function(jl_base_module, "show"), jl_stdout_obj(), p);
    jl_printf(jl_stderr_stream(), "\n");
    juliaapi_check_exception();
}
