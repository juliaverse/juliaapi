#' @export
print.jl_value <- function(s) {
    if (jl_is_null_xptr(s)) {
        stop("unexpected null pointer")
    }
    .Call("juliaapi_show", PACKAGE = "juliaapi", s)
    invisible(s)
}

#' @export
jl_eval_string <- function(str) {
    .Call("juliaapi_eval_string", PACKAGE = "juliaapi", str)
}
