#ifndef JULIAAPI_H
#define JULIAAPI_H

#include "julia.h"

namespace libjulia {

std::string get_last_loaded_symbol();
std::string get_last_dl_error_message();

JL_EXTERN void* libjulia_t;
bool load_libjulia(const std::string& libpath);
bool unload_libjulia();
bool load_libjulia_symbols();
bool load_libjulia_constants();

}


#endif
