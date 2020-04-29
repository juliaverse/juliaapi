null_jl_value_xptr <- function() {
    p <- methods::new("externalptr")
    class(p) <- "jl_value"
    p
}

set_jl_value_xptr <- function(symbol) {
    name <- substitute(symbol)
    eval(substitute(jl_set_xptr(x, y), list(x = name, y = as.character(name))))
}

jl_set_xptr <- function(xptr, symbol) {
    .Call(C_juliaapi_set_xptr, xptr, symbol)
    invisible(NULL)
}

jl_is_null_ptr <- function(t) {
    .Call(C_juliaapi_is_null_ptr, t)
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
