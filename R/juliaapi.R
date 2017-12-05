#' @name juliaapi
#' @docType package
#' @useDynLib juliaapi
#' @import Rcpp
#' @import xptr
NULL

null_jl_value_ptr <- function() {
    p <- xptr::null_xptr()
    class(p) <- "jl_value"
    p
}

#' @export
jl_any_type <- null_jl_value_ptr()
#' @export
jl_nothing <- null_jl_value_ptr()
#' @export
jl_true <- null_jl_value_ptr()
#' @export
jl_false <- null_jl_value_ptr()
#' @export
jl_main_module <- null_jl_value_ptr()
#' @export
jl_core_module <- null_jl_value_ptr()
#' @export
jl_base_module <- null_jl_value_ptr()


#' @export
jl_init <- function() {
    libjulia <- system2(
        "julia", shQuote(c("-e", 'print(Libdl.dlpath("libjulia"))')), stdout = TRUE)

    juliaapi_init(libjulia)

    load_jl_value_xptr(jl_any_type, "jl_any_type")
    load_jl_value_xptr(jl_nothing, "jl_nothing")
    load_jl_value_xptr(jl_true, "jl_true")
    load_jl_value_xptr(jl_false, "jl_false")
    load_jl_value_xptr(jl_main_module, "jl_main_module")
    load_jl_value_xptr(jl_core_module, "jl_core_module")
    load_jl_value_xptr(jl_base_module, "jl_base_module")

    invisible(TRUE)
}

#' @export
print.jl_value <- function(s) {
    if (xptr::is_null_xptr(s)) {
        stop("unexpected null pointer")
    }
    juliaapi_print(s)
}
