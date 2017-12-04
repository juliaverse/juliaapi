#define JULIA_CPP
#include "../inst/include/juliaapi.h"

#ifndef _WIN32
#include <dlfcn.h>
#else
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <string>

using namespace julia;

namespace libjulia {

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

bool load_symbol(const std::string& name, void** ppSymbol) {

    last_loaded_symbol = name;
    *ppSymbol = NULL;
#ifdef _WIN32
    *ppSymbol = (void*)::GetProcAddress((HINSTANCE) libjulia_t, name.c_str());
#else
    *ppSymbol = ::dlsym(libjulia_t, name.c_str());
#endif
    if (*ppSymbol == NULL) {
        return false;
    } else {
        return true;
    }
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

#define LOAD_JULIA_SYMBOL_AS(name, as) \
if (!load_symbol(#name, (void**) &as)) \
    return false;

#define LOAD_JULIA_SYMBOL(name) \
if (!load_symbol(#name, (void**) &name)) \
    return false;

bool load_libjulia_symbols() {
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

bool load_libjulia_constants() {
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
