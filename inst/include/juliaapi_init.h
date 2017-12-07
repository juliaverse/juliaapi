#ifndef JULIAAPI_INIT_H
#define JULIAAPI_INIT_H

#define JULIAAPI_INIT
#include "julia.h"

#include <Rcpp.h>

#include <string>
#ifndef _WIN32
#include <dlfcn.h>
#else
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

static void* libjulia_t;

static std::string last_loaded_symbol;

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

bool load_libjulia(const std::string& libpath) {
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

bool unload_libjulia() {
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

#ifdef JULIAAPI_INTERNAL

bool load_libjulia_symbol(const std::string& name, void** ppSymbol) {

    last_loaded_symbol = name;
    *ppSymbol = NULL;

#ifdef _WIN32
    *ppSymbol = (void*) GetProcAddress((HINSTANCE) libjulia_t, name.c_str());
#else
    *ppSymbol = dlsym(libjulia_t, name.c_str());
#endif

    if (*ppSymbol == NULL) {
        return false;
    } else {
        return true;
    }
}

bool load_libjulia_constant(const std::string& name, void** ppSymbol) {

    last_loaded_symbol = name;
    *ppSymbol = NULL;

#ifdef _WIN32
    *ppSymbol = *((void**) GetProcAddress((HINSTANCE) libjulia_t, name.c_str()));
#else
    *ppSymbol = *((void**) dlsym(libjulia_t, name.c_str()));
#endif

    if (*ppSymbol == NULL) {
        return false;
    } else {
        return true;
    }
}

#else

bool (*load_libjulia_symbol)(const std::string& name, void** ppSymbol);
bool (*load_libjulia_constant)(const std::string& name, void** ppSymbol);

#endif

#define LOAD_LIBJULIA_SYMBOL_AS(name, as) \
if (!load_libjulia_symbol(#name, (void**) &as)) \
    return false;

#define LOAD_LIBJULIA_SYMBOL(name) \
if (!load_libjulia_symbol(#name, (void**) &name)) \
    return false;


bool load_libjulia_symbols() {
    LOAD_LIBJULIA_SYMBOL(jl_get_ptls_states);

    LOAD_LIBJULIA_SYMBOL(jl_typeof_str);

    LOAD_LIBJULIA_SYMBOL(jl_symbol);
    LOAD_LIBJULIA_SYMBOL(jl_box_voidpointer);
    LOAD_LIBJULIA_SYMBOL(jl_unbox_voidpointer);

    LOAD_LIBJULIA_SYMBOL(jl_alloc_array_1d);
    LOAD_LIBJULIA_SYMBOL(jl_apply_array_type);
    LOAD_LIBJULIA_SYMBOL(jl_arrayset);
    LOAD_LIBJULIA_SYMBOL(jl_arrayunset);
    LOAD_LIBJULIA_SYMBOL(jl_array_grow_end);

    LOAD_LIBJULIA_SYMBOL(jl_new_module);
    LOAD_LIBJULIA_SYMBOL(jl_get_global);
    LOAD_LIBJULIA_SYMBOL(jl_set_global);
    LOAD_LIBJULIA_SYMBOL(jl_set_const);

    LOAD_LIBJULIA_SYMBOL(jl_is_initialized);
    LOAD_LIBJULIA_SYMBOL(jl_init);
    LOAD_LIBJULIA_SYMBOL(jl_atexit_hook);
    LOAD_LIBJULIA_SYMBOL(jl_eval_string);

    LOAD_LIBJULIA_SYMBOL(jl_exception_occurred);

    LOAD_LIBJULIA_SYMBOL(jl_call);
    LOAD_LIBJULIA_SYMBOL(jl_call0);
    LOAD_LIBJULIA_SYMBOL(jl_call1);
    LOAD_LIBJULIA_SYMBOL(jl_call2);
    LOAD_LIBJULIA_SYMBOL(jl_call3);

    LOAD_LIBJULIA_SYMBOL(jl_stdout_stream);
    LOAD_LIBJULIA_SYMBOL(jl_stdin_stream);
    LOAD_LIBJULIA_SYMBOL(jl_stderr_stream);
    LOAD_LIBJULIA_SYMBOL(jl_printf);
    LOAD_LIBJULIA_SYMBOL(jl_flush_cstdio);
    LOAD_LIBJULIA_SYMBOL(jl_stdout_obj);
    LOAD_LIBJULIA_SYMBOL(jl_stderr_obj);

    return true;
}

#define LOAD_LIBJULIA_CONSTANT_AS(name, as) \
if (!load_libjulia_constant(#name, (void**) &as)) \
    return false;

#define LOAD_LIBJULIA_CONSTANT(name) \
if (!load_libjulia_constant(#name, (void**) &name)) \
    return false;

bool load_libjulia_constants() {
    LOAD_LIBJULIA_CONSTANT(jl_any_type);
    LOAD_LIBJULIA_CONSTANT(jl_nothing);
    LOAD_LIBJULIA_CONSTANT(jl_true);
    LOAD_LIBJULIA_CONSTANT(jl_false);
    LOAD_LIBJULIA_CONSTANT(jl_main_module);
    LOAD_LIBJULIA_CONSTANT(jl_core_module);
    LOAD_LIBJULIA_CONSTANT(jl_base_module);
    LOAD_LIBJULIA_CONSTANT(jl_bool_type);
    LOAD_LIBJULIA_CONSTANT(jl_char_type);
    LOAD_LIBJULIA_CONSTANT(jl_int8_type);
    LOAD_LIBJULIA_CONSTANT(jl_uint8_type);
    LOAD_LIBJULIA_CONSTANT(jl_int16_type);
    LOAD_LIBJULIA_CONSTANT(jl_uint16_type);
    LOAD_LIBJULIA_CONSTANT(jl_int32_type);
    LOAD_LIBJULIA_CONSTANT(jl_uint32_type);
    LOAD_LIBJULIA_CONSTANT(jl_int64_type);
    LOAD_LIBJULIA_CONSTANT(jl_uint64_type);
    LOAD_LIBJULIA_CONSTANT(jl_float16_type);
    LOAD_LIBJULIA_CONSTANT(jl_float32_type);
    LOAD_LIBJULIA_CONSTANT(jl_float64_type);
    LOAD_LIBJULIA_CONSTANT(jl_floatingpoint_type);
    LOAD_LIBJULIA_CONSTANT(jl_number_type);
    LOAD_LIBJULIA_CONSTANT(jl_void_type);
    LOAD_LIBJULIA_CONSTANT(jl_signed_type);
    LOAD_LIBJULIA_CONSTANT(jl_voidpointer_type);
    return true;
}


#ifdef JULIAAPI_INTERNAL
    // load internally

    SEXP cast_xptr(jl_value_t* s, bool preserve);
    jl_value_t* cast_jl_value_t(SEXP s);
    void juliaapi_init();

#else
    // load externally

    SEXP (*cast_xptr)(jl_value_t* s, bool preserve);
    jl_value_t* (*cast_jl_value_t)(SEXP s);


    void load_juliaapi_symbol(const std::string& name, void** ppSymbol) {
        *ppSymbol = (void*) R_GetCCallable("juliaapi", name.c_str());
    }

    #define LOAD_JULIAAPI_SYMBOL(name) load_juliaapi_symbol(#name, (void**) &name)

    void juliaapi_init() {
        LOAD_JULIAAPI_SYMBOL(cast_xptr);
        LOAD_JULIAAPI_SYMBOL(cast_jl_value_t);

        LOAD_JULIAAPI_SYMBOL(juliaapi_check_exception);
        LOAD_JULIAAPI_SYMBOL(juliaapi_print);
        LOAD_JULIAAPI_SYMBOL(juliaapi_eval_string);

        LOAD_JULIAAPI_SYMBOL(load_libjulia_symbol);
        LOAD_JULIAAPI_SYMBOL(load_libjulia_constant);

        load_libjulia_symbols();
        load_libjulia_constants();
    }

#endif

#include <RcppCommon.h>

namespace Rcpp {

    SEXP wrap(jl_value_t* object) {
        return cast_xptr(object, true);
    }

    template<> jl_value_t* as(SEXP object) {
        return cast_jl_value_t(object);
    }
}

#endif
