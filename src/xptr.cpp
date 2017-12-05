#include "../inst/include/julia.h"
#include <stack>
#include <map>
#include <string>
#include <R.h>
#include <Rinternals.h>

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

void gc_preserve(jl_value_t* s) {
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

void gc_release(jl_value_t* s) {
    std::map<jl_value_t*, std::pair<size_t, size_t> >::iterator it;
    it = gc_index_map.find(s);
    if(it == gc_index_map.end()) {
        error("trying to release an unprotected julia object.");
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
    gc_release((jl_value_t*) R_ExternalPtrAddr(s));
}

SEXP cast_xptr(jl_value_t* s, bool preserve) {
    SEXP sp;
    if (preserve) {
        gc_preserve(s);
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

std::map<std::string, jl_value_t*> xptr_map() {
    std::map<std::string, jl_value_t*> m;
    m["jl_any_type"] =  jl_any_type;
    m["jl_nothing"] =  jl_nothing;
    m["jl_true"] =  jl_true;
    m["jl_false"] =  jl_false;
    m["jl_main_module"] =  jl_main_module;
    m["jl_core_module"] =  jl_core_module;
    m["jl_core_module"] =  jl_core_module;
    return m;
}
// [[Rcpp::export]]
void load_jl_value_xptr(SEXP t, const std::string& name) {
    static std::map<std::string, jl_value_t*> m = xptr_map();
    R_SetExternalPtrAddr(t, (void*) m[name]);
}
