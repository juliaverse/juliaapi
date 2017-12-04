#ifndef LIBJULIA_H
#define LIBJULIA_H

#ifndef _WIN32
#include <dlfcn.h>
#else
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include "julia.h"

using namespace julia;

namespace libjulia {

std::string get_last_loaded_symbol();
std::string get_last_dl_error_message();
bool load(const std::string& libpath);
bool unload();
bool load_symbols();
bool load_constants();

}

#endif
