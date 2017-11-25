#ifndef __LIBJULIA_HPP__
#define __LIBJULIA_HPP__

#include <string>
#include <ostream>
#include <stdint.h>

#ifndef LIBJULIA_CPP
#define JL_EXTERN extern
#else
#define JL_EXTERN
#endif

#if defined(_OS_WINDOWS_) && defined(_COMPILER_INTEL_)
#  define STATIC_INLINE static
#  define INLINE
#elif defined(_OS_WINDOWS_) && defined(_COMPILER_MICROSOFT_)
#  define STATIC_INLINE static __inline
#  define INLINE __inline
#else
#  define STATIC_INLINE static inline
#  define INLINE inline
#endif

namespace libjulia {

#define HT_N_INLINE 32
typedef struct {
    size_t size;
    void **table;
    void *_space[HT_N_INLINE];
} htable_t;

#define AL_N_INLINE 29
typedef struct {
    size_t len;
    size_t max;
    void **items;
    void *_space[AL_N_INLINE];
} arraylist_t;

#define JL_DATA_TYPE

typedef struct _jl_value_t jl_value_t;

typedef jl_value_t jl_function_t;
typedef struct _jl_sym_t {
    JL_DATA_TYPE
    struct _jl_sym_t *left;
    struct _jl_sym_t *right;
    uintptr_t hash;    // precomputed hash value
    // JL_ATTRIBUTE_ALIGN_PTRSIZE(char name[]);
} jl_sym_t;

typedef struct _jl_module_t {
    JL_DATA_TYPE
    jl_sym_t *name;
    struct _jl_module_t *parent;
    htable_t bindings;
    arraylist_t usings;  // modules with all bindings potentially imported
    uint64_t uuid;
    size_t primary_world;
    uint32_t counter;
    uint8_t istopmod;
} jl_module_t;

typedef struct uv_stream_s uv_stream_t;


// gc

// assume JULIA_ENABLE_THREADING
typedef struct _jl_tls_states_t jl_tls_states_t;
JL_EXTERN jl_tls_states_t* (*jl_get_ptls_states)(void);

typedef struct _jl_gcframe_t {
    size_t nroots;
    struct _jl_gcframe_t *prev;
    // actual roots go here
} jl_gcframe_t;

#define jl_pgcstack (jl_get_ptls_states()->pgcstack)

#define JL_GC_PUSH1(arg1)                                                 \
  void *__gc_stkf[] = {(void*)3, jl_pgcstack, arg1};                      \
  jl_pgcstack = (jl_gcframe_t*)__gc_stkf;

#define JL_GC_PUSH2(arg1, arg2)                                           \
  void *__gc_stkf[] = {(void*)5, jl_pgcstack, arg1, arg2};                \
  jl_pgcstack = (jl_gcframe_t*)__gc_stkf;

#define JL_GC_PUSH3(arg1, arg2, arg3)                                     \
  void *__gc_stkf[] = {(void*)7, jl_pgcstack, arg1, arg2, arg3};          \
  jl_pgcstack = (jl_gcframe_t*)__gc_stkf;

#define JL_GC_PUSH4(arg1, arg2, arg3, arg4)                               \
  void *__gc_stkf[] = {(void*)9, jl_pgcstack, arg1, arg2, arg3, arg4};    \
  jl_pgcstack = (jl_gcframe_t*)__gc_stkf;

#define JL_GC_PUSH5(arg1, arg2, arg3, arg4, arg5)                               \
  void *__gc_stkf[] = {(void*)11, jl_pgcstack, arg1, arg2, arg3, arg4, arg5};    \
  jl_pgcstack = (jl_gcframe_t*)__gc_stkf;

#define JL_GC_PUSH6(arg1, arg2, arg3, arg4, arg5, arg6)                      \
  void *__gc_stkf[] = {(void*)13, jl_pgcstack, arg1, arg2, arg3, arg4, arg5, arg6}; \
  jl_pgcstack = (jl_gcframe_t*)__gc_stkf;

#define JL_GC_PUSHARGS(rts_var,n)                               \
  rts_var = ((jl_value_t**)alloca(((n)+2)*sizeof(jl_value_t*)))+2;    \
  ((void**)rts_var)[-2] = (void*)(((size_t)(n))<<1);                  \
  ((void**)rts_var)[-1] = jl_pgcstack;                          \
  memset((void*)rts_var, 0, (n)*sizeof(jl_value_t*));           \
  jl_pgcstack = (jl_gcframe_t*)&(((void**)rts_var)[-2])

#define JL_GC_POP() (jl_pgcstack = jl_pgcstack->prev)

// type predicates and basic operations
JL_EXTERN const char* (*jl_typeof_str)(jl_value_t *v);

// constructors
JL_EXTERN jl_sym_t* (*jl_symbol)(const char *str);
JL_EXTERN jl_value_t* (*jl_box_voidpointer)(void *x);
JL_EXTERN void* (*jl_unbox_voidpointer)(jl_value_t *v);

// modules and global variables
JL_EXTERN jl_module_t* jl_main_module;
JL_EXTERN jl_module_t* jl_core_module;
JL_EXTERN jl_module_t* jl_base_module;
JL_EXTERN jl_value_t* (*jl_get_global)(jl_module_t *m, jl_sym_t *var);
STATIC_INLINE jl_function_t *jl_get_function(jl_module_t *m, const char *name)
{
    return (jl_function_t*)jl_get_global(m, jl_symbol(name));
}

// initialization functions
JL_EXTERN int (*jl_is_initialized)(void);
JL_EXTERN void (*jl_init)(void);
JL_EXTERN void (*jl_atexit_hook)(int status);

// front end interface
JL_EXTERN jl_value_t* (*jl_eval_string)(const char *str);

// throwing common exceptions
JL_EXTERN jl_value_t* (*jl_exception_occurred)(void);

// calling into julia
JL_EXTERN jl_value_t* (*jl_call)(jl_function_t *f, jl_value_t **args, int32_t nargs);
JL_EXTERN jl_value_t* (*jl_call0)(jl_function_t *f);
JL_EXTERN jl_value_t* (*jl_call1)(jl_function_t *f, jl_value_t *a);
JL_EXTERN jl_value_t* (*jl_call2)(jl_function_t *f, jl_value_t *a, jl_value_t *b);
JL_EXTERN jl_value_t* (*jl_call3)(jl_function_t *f, jl_value_t *a,
                                  jl_value_t *b, jl_value_t *c);

// I/O system

JL_EXTERN uv_stream_t* (*jl_stderr_stream)(void);

#ifdef __GNUC__
#define _JL_FORMAT_ATTR(type, str, arg) \
    __attribute__((format(type, str, arg)))
#else
#define _JL_FORMAT_ATTR(type, str, arg)
#endif

JL_EXTERN int (*jl_printf)(uv_stream_t *s, const char *format, ...)
    _JL_FORMAT_ATTR(printf, 2, 3);

// showing and std streams
JL_EXTERN void (*jl_flush_cstdio)(void);
JL_EXTERN jl_value_t* (*jl_stdout_obj)(void);
JL_EXTERN jl_value_t* (*jl_stderr_obj)(void);


std::string get_last_loaded_symbol();
std::string get_last_dl_error_message();

JL_EXTERN void* libjulia_t;
bool load_libjulia(const std::string& libpath);
bool unload_libjulia();
bool load_libjulia_symbols();
bool load_libjulia_modules();

} // namespace libjulia

#endif
