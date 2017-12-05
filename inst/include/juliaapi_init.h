#ifndef JULIAAPI_INIT_H
#define JULIAAPI_INIT_H

#define JULIAAPI_INIT
#include "julia.h"

#include <RcppCommon.h>

#include <string>
#ifndef _WIN32
#include <dlfcn.h>
#else
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

namespace libjulia {

void* libjulia_t;

void* get_libjulia_t() {
    return libjulia_t;
}

std::string last_loaded_symbol;

std::string get_last_loaded_symbol() {
    return last_loaded_symbol;
}

std::string get_last_dl_error_message() {
    std::string Error;
#ifdef _WIN32
    LPVOID lpMsgBuf;
    DWORD dw = ::GetLastError();

    DWORD length = ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    if (length != 0) {
        std::string msg((LPTSTR)lpMsgBuf);
        LocalFree(lpMsgBuf);
        Error.assign(msg);
    } else {
        Error.assign("(Unknown error)");
    }
#else
    const char* msg = ::dlerror();
    if (msg != NULL)
        Error.assign(msg);
    else
        Error.assign("(Unknown error)");
#endif
    return Error;
}

bool load(const std::string& libpath) {
    libjulia_t = NULL;
#ifdef _WIN32
    libjulia_t = (void*)::LoadLibraryEx(libpath.c_str(), NULL, 0);
#else
    libjulia_t = ::dlopen(libpath.c_str(), RTLD_NOW|RTLD_GLOBAL);
#endif
    if (libjulia_t == NULL) {
        return false;
    } else {
        return true;
    }
}

bool unload() {
  if (libjulia_t != NULL) {
#ifdef _WIN32
    if (!::FreeLibrary((HMODULE)libjulia_t)) {
#else
    if (::dlclose(libjulia_t) != 0) {
#endif
      return false;
    } else {
      return true;
    }
  }
  else
    return true;
}

bool load_symbol(const std::string& name, void** ppSymbol) {

    last_loaded_symbol = name;
    *ppSymbol = NULL;

#ifdef JULIAAPI_CPP

#ifdef _WIN32
    *ppSymbol = (void*)::GetProcAddress((HINSTANCE) libjulia_t, name.c_str());
#else
    *ppSymbol = ::dlsym(libjulia_t, name.c_str());
#endif

#else
    *ppSymbol = (void*) R_GetCCallable("juliaapi", name.c_str());

#endif
    if (*ppSymbol == NULL) {
        return false;
    } else {
        return true;
    }
}

#ifdef JULIAAPI_CPP
// when loaded internally

#define LOAD_JULIA_SYMBOL_AS(name, as) \
if (!load_symbol(#name, (void**) &as)) \
    return false;                      \
R_RegisterCCallable("juliaapi", #name, (DL_FUNC) as);

#define LOAD_JULIA_SYMBOL(name) \
if (!load_symbol(#name, (void**) &name)) \
    return false;               \
R_RegisterCCallable("juliaapi", #name, (DL_FUNC) name);

#else

// when loaded externally

#define LOAD_JULIA_SYMBOL_AS(name, as) \
if (!load_symbol(#name, (void**) &as)) \
    return false;

#define LOAD_JULIA_SYMBOL(name) \
if (!load_symbol(#name, (void**) &name)) \
    return false;

#endif

bool load_symbols() {
    LOAD_JULIA_SYMBOL(jl_get_ptls_states);

    LOAD_JULIA_SYMBOL(jl_typeof_str);

    LOAD_JULIA_SYMBOL(jl_symbol);
    LOAD_JULIA_SYMBOL(jl_box_voidpointer);
    LOAD_JULIA_SYMBOL(jl_unbox_voidpointer);

    LOAD_JULIA_SYMBOL(jl_alloc_array_1d);
    LOAD_JULIA_SYMBOL(jl_apply_array_type);
    LOAD_JULIA_SYMBOL(jl_arrayset);
    LOAD_JULIA_SYMBOL(jl_arrayunset);
    LOAD_JULIA_SYMBOL(jl_array_grow_end);

    LOAD_JULIA_SYMBOL(jl_new_module);
    LOAD_JULIA_SYMBOL(jl_get_global);
    LOAD_JULIA_SYMBOL(jl_set_global);
    LOAD_JULIA_SYMBOL(jl_set_const);

    LOAD_JULIA_SYMBOL(jl_is_initialized);
    LOAD_JULIA_SYMBOL(jl_init);
    LOAD_JULIA_SYMBOL(jl_atexit_hook);
    LOAD_JULIA_SYMBOL(jl_eval_string);

    LOAD_JULIA_SYMBOL(jl_exception_occurred);

    LOAD_JULIA_SYMBOL(jl_call);
    LOAD_JULIA_SYMBOL(jl_call0);
    LOAD_JULIA_SYMBOL(jl_call1);
    LOAD_JULIA_SYMBOL(jl_call2);
    LOAD_JULIA_SYMBOL(jl_call3);

    LOAD_JULIA_SYMBOL(jl_stdout_stream);
    LOAD_JULIA_SYMBOL(jl_stdin_stream);
    LOAD_JULIA_SYMBOL(jl_stderr_stream);
    LOAD_JULIA_SYMBOL(jl_printf);
    LOAD_JULIA_SYMBOL(jl_flush_cstdio);
    LOAD_JULIA_SYMBOL(jl_stdout_obj);
    LOAD_JULIA_SYMBOL(jl_stderr_obj);

    return true;
}

bool load_constants() {
    // not sure why LOAD_JULIA_SYMBOL fails

    jl_any_type = jl_eval_string("Any");
    jl_true = jl_eval_string("true");
    jl_false = jl_eval_string("false");
    jl_nothing = jl_eval_string("nothing");

    jl_main_module = (jl_module_t*) jl_eval_string("Main");
    jl_core_module = (jl_module_t*) jl_eval_string("Core");
    jl_base_module = (jl_module_t*) jl_eval_string("Base");
    return true;
}

} // namespace julia


#ifdef JULIAAPI_CPP
// load internally

SEXP cast_xptr(jl_value_t* s, bool preserve);
jl_value_t* cast_jl_value_t(SEXP s);
void juliaapi_check_exception();
void juliaapi_print(jl_value_t* t);
SEXP juliaapi_eval_string(const char* str, bool preserve);
void juliaapi_init();

#else
// load externally

SEXP (*cast_xptr)(jl_value_t* s, bool preserve);
jl_value_t* (*cast_jl_value_t)(SEXP s);
void (*juliaapi_check_exception)();
void (*juliaapi_print)(jl_value_t* t);
SEXP (*juliaapi_eval_string)(const char* str, bool preserve);

void juliaapi_init() {
    libjulia::load_symbol("cast_xptr", (void**) &cast_xptr);
    libjulia::load_symbol("cast_jl_value_t", (void**) &cast_jl_value_t);

    libjulia::load_symbol("juliaapi_check_exception", (void**) &juliaapi_check_exception);
    libjulia::load_symbol("juliaapi_print", (void**) &juliaapi_print);
    libjulia::load_symbol("juliaapi_eval_string", (void**) &juliaapi_eval_string);

    libjulia::load_symbols();
    libjulia::load_constants();
}

#endif

namespace Rcpp {

    SEXP wrap(jl_value_t* object) {
        return cast_xptr(object, true);
    }

    template<> jl_value_t* as(SEXP object) {
        return cast_jl_value_t(object);
    }
}

#endif
