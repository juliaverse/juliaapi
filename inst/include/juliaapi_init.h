#ifndef JULIAAPI_INIT_H
#define JULIAAPI_INIT_H

#include <stdlib.h>
#include <string.h>

#define JULIAAPI
#include "julia.h"

#include "juliaapi.h"

#ifndef _WIN32
#include <dlfcn.h>
#else
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

static void* libjulia_t;

static char last_loaded_symbol[100] = "";

char* get_last_loaded_symbol() {
    return last_loaded_symbol;
}

static char dl_error_message[1024] = "";

char* get_dl_error_message() {
#ifdef _WIN32
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError();

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
        strcpy(dl_error_message, lpMsgBuf);
        LocalFree(lpMsgBuf);
    } else {
        strcpy(dl_error_message, "(Unknown error)");
    }
#else
    char* msg = dlerror();
    if (msg != NULL)
        strcpy(dl_error_message, msg);
    else
        strcpy(dl_error_message, "(Unknown error)");
#endif
    return dl_error_message;
}

bool load_julia(const char* libpath) {
    libjulia_t = NULL;
#ifdef _WIN32
    libjulia_t = (void*)LoadLibraryEx(libpath, NULL, 0);
#else
    libjulia_t = dlopen(libpath, RTLD_NOW|RTLD_GLOBAL);
#endif
    if (libjulia_t == NULL) {
        return false;
    } else {
        return true;
    }
}

bool unload_julia() {
  if (libjulia_t != NULL) {
#ifdef _WIN32
    if (!FreeLibrary((HMODULE)libjulia_t)) {
#else
    if (dlclose(libjulia_t) != 0) {
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

bool load_julia_symbol(const char* name, void** ppSymbol) {

    strcpy(last_loaded_symbol, name);
    *ppSymbol = NULL;

#ifdef _WIN32
    *ppSymbol = (void*) GetProcAddress((HINSTANCE) libjulia_t, name);
#else
    *ppSymbol = dlsym(libjulia_t, name);
#endif

    if (*ppSymbol == NULL) {
        return false;
    } else {
        return true;
    }
}

bool load_julia_constant(const char* name, void** ppSymbol) {

    strcpy(last_loaded_symbol, name);
    *ppSymbol = NULL;

#ifdef _WIN32
    *ppSymbol = *((void**) GetProcAddress((HINSTANCE) libjulia_t, name));
#else
    *ppSymbol = *((void**) dlsym(libjulia_t, name));
#endif

    if (*ppSymbol == NULL) {
        return false;
    } else {
        return true;
    }
}

#else

bool (*load_julia_symbol)(const char* name, void** ppSymbol);
bool (*load_julia_constant)(const char* name, void** ppSymbol);

#endif


#define LOAD_JULIA_SYMBOL_AS(name, as) \
if (!load_julia_symbol(#name, (void**) &as)) \
    return false;

#define LOAD_JULIA_SYMBOL(name) \
if (!load_julia_symbol(#name, (void**) &name)) \
    return false;


bool load_julia_symbols() {
    LOAD_JULIA_SYMBOL(jl_get_ptls_states);

    LOAD_JULIA_SYMBOL(jl_isa);
    LOAD_JULIA_SYMBOL(jl_types_equal);
    LOAD_JULIA_SYMBOL(jl_typename_str);
    LOAD_JULIA_SYMBOL(jl_typeof_str);

    LOAD_JULIA_SYMBOL(jl_symbol);

    LOAD_JULIA_SYMBOL(jl_box_bool);
    LOAD_JULIA_SYMBOL(jl_box_int8);
    LOAD_JULIA_SYMBOL(jl_box_uint8);
    LOAD_JULIA_SYMBOL(jl_box_int16);
    LOAD_JULIA_SYMBOL(jl_box_uint16);
    LOAD_JULIA_SYMBOL(jl_box_int32);
    LOAD_JULIA_SYMBOL(jl_box_uint32);
    LOAD_JULIA_SYMBOL(jl_box_char);
    LOAD_JULIA_SYMBOL(jl_box_int64);
    LOAD_JULIA_SYMBOL(jl_box_uint64);
    LOAD_JULIA_SYMBOL(jl_box_float32);
    LOAD_JULIA_SYMBOL(jl_box_float64);
    LOAD_JULIA_SYMBOL(jl_box_voidpointer);

    LOAD_JULIA_SYMBOL(jl_unbox_bool);
    LOAD_JULIA_SYMBOL(jl_unbox_int8);
    LOAD_JULIA_SYMBOL(jl_unbox_uint8);
    LOAD_JULIA_SYMBOL(jl_unbox_int16);
    LOAD_JULIA_SYMBOL(jl_unbox_uint16);
    LOAD_JULIA_SYMBOL(jl_unbox_int32);
    LOAD_JULIA_SYMBOL(jl_unbox_uint32);
    LOAD_JULIA_SYMBOL(jl_unbox_int64);
    LOAD_JULIA_SYMBOL(jl_unbox_uint64);
    LOAD_JULIA_SYMBOL(jl_unbox_float32);
    LOAD_JULIA_SYMBOL(jl_unbox_float64);
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

    if (!load_julia_symbol("jl_init", (void**) &jl_init) && \
            !load_julia_symbol("jl_init__threading", (void**) &jl_init))
        return false;

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

#define LOAD_JULIA_CONSTANT_AS(name, as) \
if (!load_julia_constant(#name, (void**) &as)) \
    return false;

#define LOAD_JULIA_CONSTANT(name) \
if (!load_julia_constant(#name, (void**) &name)) \
    return false;

bool load_julia_constants() {
    LOAD_JULIA_CONSTANT(jl_any_type);
    LOAD_JULIA_CONSTANT(jl_nothing);
    LOAD_JULIA_CONSTANT(jl_true);
    LOAD_JULIA_CONSTANT(jl_false);
    LOAD_JULIA_CONSTANT(jl_main_module);
    LOAD_JULIA_CONSTANT(jl_core_module);
    LOAD_JULIA_CONSTANT(jl_base_module);
    LOAD_JULIA_CONSTANT(jl_bool_type);
    LOAD_JULIA_CONSTANT(jl_char_type);
    LOAD_JULIA_CONSTANT(jl_int8_type);
    LOAD_JULIA_CONSTANT(jl_uint8_type);
    LOAD_JULIA_CONSTANT(jl_int16_type);
    LOAD_JULIA_CONSTANT(jl_uint16_type);
    LOAD_JULIA_CONSTANT(jl_int32_type);
    LOAD_JULIA_CONSTANT(jl_uint32_type);
    LOAD_JULIA_CONSTANT(jl_int64_type);
    LOAD_JULIA_CONSTANT(jl_uint64_type);
    LOAD_JULIA_CONSTANT(jl_float16_type);
    LOAD_JULIA_CONSTANT(jl_float32_type);
    LOAD_JULIA_CONSTANT(jl_float64_type);
    LOAD_JULIA_CONSTANT(jl_floatingpoint_type);
    LOAD_JULIA_CONSTANT(jl_number_type);
    LOAD_JULIA_CONSTANT(jl_void_type);
    LOAD_JULIA_CONSTANT(jl_signed_type);
    LOAD_JULIA_CONSTANT(jl_voidpointer_type);
    return true;
}


#ifdef JULIAAPI_INTERNAL
    // load internally

    #define REGISTER_JULIAAPI_SYMBOL(name) \
        R_RegisterCCallable("juliaapi", #name, (DL_FUNC) name);

    int juliaapi_init() {
        REGISTER_JULIAAPI_SYMBOL(load_julia_symbol);
        REGISTER_JULIAAPI_SYMBOL(load_julia_constant);
        REGISTER_JULIAAPI_SYMBOL(juliaapi_is_initialized);
        REGISTER_JULIAAPI_SYMBOL(juliaapi_check_exception);
        REGISTER_JULIAAPI_SYMBOL(juliaapi_show);
        REGISTER_JULIAAPI_SYMBOL(cast_xptr);
        REGISTER_JULIAAPI_SYMBOL(cast_jl_value_t);

        if (!load_julia_symbols()) return false;
        jl_init();
        if (!load_julia_constants()) return false;

        return true;
    }

#else
    // load externally

    void load_juliaapi_symbol(char* name, void** ppSymbol) {
        *ppSymbol = (void*) R_GetCCallable("juliaapi", name);
    }

    #define LOAD_JULIAAPI_SYMBOL(name) load_juliaapi_symbol(#name, (void**) &name)

    int juliaapi_init() {
        LOAD_JULIAAPI_SYMBOL(load_julia_symbol);
        LOAD_JULIAAPI_SYMBOL(load_julia_constant);
        LOAD_JULIAAPI_SYMBOL(juliaapi_is_initialized);
        LOAD_JULIAAPI_SYMBOL(juliaapi_check_exception);
        LOAD_JULIAAPI_SYMBOL(juliaapi_show);
        LOAD_JULIAAPI_SYMBOL(cast_xptr);
        LOAD_JULIAAPI_SYMBOL(cast_jl_value_t);

        if (!load_julia_symbols()) return false;
        if (!load_julia_constants()) return false;

        return true;
    }

#endif

#endif
