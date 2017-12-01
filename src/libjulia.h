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

#define STORE_ARRAY_LEN
#define JULIA_ENABLE_THREADING

typedef struct _jl_value_t jl_value_t;

typedef jl_value_t jl_function_t;
typedef jl_value_t jl_sym_t;

typedef struct {
    /*
      how - allocation style
      0 = data is inlined, or a foreign pointer we don't manage
      1 = julia-allocated buffer that needs to be marked
      2 = malloc-allocated pointer this array object manages
      3 = has a pointer to the object that owns the data
    */
    uint16_t how:2;
    uint16_t ndims:10;
    uint16_t pooled:1;
    uint16_t ptrarray:1;  // representation is pointer array
    uint16_t isshared:1;  // data is shared by multiple Arrays
    uint16_t isaligned:1; // data allocated with memalign
} jl_array_flags_t;

typedef struct {
    void *data;
#ifdef STORE_ARRAY_LEN
    size_t length;
#endif
    jl_array_flags_t flags;
    uint16_t elsize;
    uint32_t offset;  // for 1-d only. does not need to get big.
    size_t nrows;
    union {
        // 1d
        size_t maxsize;
        // Nd
        size_t ncols;
    };
    // other dim sizes go here for ndims > 2

    // followed by alignment padding and inline data, or owner pointer
} jl_array_t;
typedef jl_value_t jl_module_t;
typedef struct uv_stream_s uv_stream_t;

// gc
typedef struct _jl_gcframe_t {
    size_t nroots;
    struct _jl_gcframe_t *prev;
} jl_gcframe_t;

typedef struct _jl_tls_states_t {
    struct _jl_gcframe_t *pgcstack;
} jl_tls_states_t;

// assume JULIA_ENABLE_THREADING
JL_EXTERN jl_tls_states_t* (*jl_get_ptls_states)(void);

#define jl_pgcstack (jl_get_ptls_states()->pgcstack)

#define JL_GC_PUSH1(arg1)                                                 \
  void *__gc_stkf[] = {(void*)3, jl_pgcstack, arg1};                      \
  jl_pgcstack = (jl_gcframe_t*)__gc_stkf;

#define JL_GC_POP() (jl_pgcstack = jl_pgcstack->prev)

// object accessors
#define jl_array_len(a)   (((jl_array_t*)(a))->length)
#define jl_array_data(a)  ((void*)((jl_array_t*)(a))->data)
#define jl_array_dim(a,i) ((&((jl_array_t*)(a))->nrows)[i])
#define jl_array_dim0(a)  (((jl_array_t*)(a))->nrows)
#define jl_array_nrows(a) (((jl_array_t*)(a))->nrows)
#define jl_array_ndims(a) ((int32_t)(((jl_array_t*)a)->flags.ndims))

// constants
JL_EXTERN jl_value_t* jl_any_type;
JL_EXTERN jl_value_t* jl_true;
JL_EXTERN jl_value_t* jl_false;
JL_EXTERN jl_value_t* jl_nothing;

// type predicates and basic operations
JL_EXTERN const char* (*jl_typeof_str)(jl_value_t *v);

// constructors
JL_EXTERN jl_sym_t* (*jl_symbol)(const char *str);
JL_EXTERN jl_value_t* (*jl_box_voidpointer)(void *x);
JL_EXTERN void* (*jl_unbox_voidpointer)(jl_value_t *v);

// arrays
JL_EXTERN jl_array_t* (*jl_alloc_array_1d)(jl_value_t *atype, size_t nr);
JL_EXTERN jl_value_t* (*jl_apply_array_type)(jl_value_t *type, size_t dim);
JL_EXTERN void (*jl_arrayset)(jl_array_t *a, jl_value_t *v, size_t i);  // 0-indexed
JL_EXTERN void (*jl_arrayunset)(jl_array_t *a, size_t i);  // 0-indexed
JL_EXTERN void (*jl_array_grow_end)(jl_array_t *a, size_t inc);

// modules and global variables
JL_EXTERN jl_module_t* jl_main_module;
JL_EXTERN jl_module_t* jl_core_module;
JL_EXTERN jl_module_t* jl_base_module;
JL_EXTERN jl_module_t* (*jl_new_module)(jl_sym_t *name);
JL_EXTERN jl_value_t* (*jl_get_global)(jl_module_t *m, jl_sym_t *var);
STATIC_INLINE jl_function_t *jl_get_function(jl_module_t *m, const char *name)
{
    return (jl_function_t*)jl_get_global(m, jl_symbol(name));
}
JL_EXTERN void (*jl_set_global)(jl_module_t *m, jl_sym_t *var, jl_value_t *val);
JL_EXTERN void (*jl_set_const)(jl_module_t *m, jl_sym_t *var, jl_value_t *val);

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

JL_EXTERN uv_stream_t* (*jl_stdout_stream)(void);
JL_EXTERN uv_stream_t* (*jl_stdin_stream)(void);
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
bool load_libjulia_constants();

} // namespace libjulia

#endif
