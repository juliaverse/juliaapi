#' @name juliaapi-package
#' @title Expose Julia API
#' @docType package
#' @useDynLib juliaapi
#' @import Rcpp
#' @import methods
NULL

null_jl_value_xptr <- function() {
    p <- methods::new("externalptr")
    class(p) <- "jl_value"
    p
}

set_jl_value_xptr <- function(symbol) {
    name <- substitute(symbol)
    eval(substitute(juliaapi_set_xptr(x, y), list(x = name, y = as.character(name))))
}

#' @export
jl_any_type <- null_jl_value_xptr()
#' @export
jl_nothing <- null_jl_value_xptr()
#' @export
jl_true <- null_jl_value_xptr()
#' @export
jl_false <- null_jl_value_xptr()
#' @export
jl_main_module <- null_jl_value_xptr()
#' @export
jl_core_module <- null_jl_value_xptr()
#' @export
jl_base_module <- null_jl_value_xptr()
#' @export
jl_bool_type <- null_jl_value_xptr()
#' @export
jl_char_type <- null_jl_value_xptr()
#' @export
jl_int8_type <- null_jl_value_xptr()
#' @export
jl_uint8_type <- null_jl_value_xptr()
#' @export
jl_int16_type <- null_jl_value_xptr()
#' @export
jl_uint16_type <- null_jl_value_xptr()
#' @export
jl_int32_type <- null_jl_value_xptr()
#' @export
jl_uint32_type <- null_jl_value_xptr()
#' @export
jl_int64_type <- null_jl_value_xptr()
#' @export
jl_uint64_type <- null_jl_value_xptr()
#' @export
jl_float16_type <- null_jl_value_xptr()
#' @export
jl_float32_type <- null_jl_value_xptr()
#' @export
jl_float64_type <- null_jl_value_xptr()
#' @export
jl_floatingpoint_type <- null_jl_value_xptr()
#' @export
jl_number_type <- null_jl_value_xptr()
#' @export
jl_void_type <- null_jl_value_xptr()
#' @export
jl_signed_type <- null_jl_value_xptr()
#' @export
jl_voidpointer_type <- null_jl_value_xptr()


juliaapi_init_contants <- function() {
    set_jl_value_xptr(jl_any_type)
    set_jl_value_xptr(jl_nothing)
    set_jl_value_xptr(jl_true)
    set_jl_value_xptr(jl_false)
    set_jl_value_xptr(jl_main_module)
    set_jl_value_xptr(jl_core_module)
    set_jl_value_xptr(jl_base_module)
    set_jl_value_xptr(jl_bool_type)
    set_jl_value_xptr(jl_char_type)
    set_jl_value_xptr(jl_int8_type)
    set_jl_value_xptr(jl_uint8_type)
    set_jl_value_xptr(jl_int16_type)
    set_jl_value_xptr(jl_uint16_type)
    set_jl_value_xptr(jl_int32_type)
    set_jl_value_xptr(jl_uint32_type)
    set_jl_value_xptr(jl_int64_type)
    set_jl_value_xptr(jl_uint64_type)
    set_jl_value_xptr(jl_float16_type)
    set_jl_value_xptr(jl_float32_type)
    set_jl_value_xptr(jl_float64_type)
    set_jl_value_xptr(jl_floatingpoint_type)
    set_jl_value_xptr(jl_number_type)
    set_jl_value_xptr(jl_void_type)
    set_jl_value_xptr(jl_signed_type)
    set_jl_value_xptr(jl_voidpointer_type)
}


jl_check_initialized <- function() {
    juliaapi_is_initialized() || jl_init()
}

#' @export
jl_is_initialized <- function() {
    juliaapi_is_initialized()
}

#' @export
jl_init <- function() {
    libjulia <- system2(
        "julia", shQuote(c("-e", 'print(Libdl.dlpath("libjulia"))')), stdout = TRUE)

    juliaapi_init(libjulia)
    juliaapi_init_contants()

    invisible(TRUE)
}

#' @export
print.jl_value <- function(s) {
    if (juliaapi_is_null_xptr(s)) {
        stop("unexpected null pointer")
    }
    juliaapi_print(s)
}

#' @export
jl_eval_string <- function(s) {
    jl_check_initialized()
    juliaapi_eval_string(s)
}

#' @export
jl_get_function <- function(module, fname) {
    jl_check_initialized()
    juliaapi_get_function(module, fname)
}

#' @export
jl_call <- function(f, ...) {
    jl_check_initialized()
    juliaapi_call(f, list(...))
}

#' @export
jl_call0 <- function(f) {
    jl_check_initialized()
    juliaapi_call0(f)
}

#' @export
jl_call1 <- function(f, a) {
    jl_check_initialized()
    juliaapi_call1(f, a)
}

#' @export
jl_call2 <- function(f, a, b) {
    jl_check_initialized()
    juliaapi_call2(f, a, b)
}

#' @export
jl_call3 <- function(f, a, b, c) {
    jl_check_initialized()
    juliaapi_call3(f, a, b, c)
}
