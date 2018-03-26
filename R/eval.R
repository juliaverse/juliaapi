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

#' @export
jl_call0 <- function(f) {
    .Call("juliaapi_call0", PACKAGE = "juliaapi", f)
}

#' @export
jl_call1 <- function(f, a) {
    .Call("juliaapi_call1", PACKAGE = "juliaapi", f, a)
}


#' @export
jl_call2 <- function(f, a) {
    .Call("juliaapi_call2", PACKAGE = "juliaapi", f, a, b)
}


#' @export
jl_call3 <- function(str) {
    .Call("juliaapi_call3", PACKAGE = "juliaapi", f, a, b, c)
}
