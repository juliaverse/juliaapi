#' @name juliaapi-package
#' @title Expose Julia API
#' @docType package
#' @useDynLib juliaapi
#' @import Rcpp
#' @import methods
NULL

#' @export
print.jl_value <- function(s) {
    if (jl_is_null_xptr(s)) {
        stop("unexpected null pointer")
    }
    jl_show(s)
}

#' @export
jl_call <- function(f, ...) {
    juliaapi_call(f, list(...))
}
