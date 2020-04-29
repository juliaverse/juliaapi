#' @export
print.jl_value <- function(s) {
    if (jl_is_null_ptr(s)) {
        stop("unexpected null pointer")
    }
    .Call(C_juliaapi_show, s)
    invisible(s)
}

#' @export
jl_eval_string <- function(str) {
    .Call(C_juliaapi_eval_string, str)
}

#' @export
jl_call0 <- function(f) {
    .Call(C_juliaapi_call0, f)
}

#' @export
jl_call1 <- function(f, a) {
    .Call(C_juliaapi_call1, f, a)
}


#' @export
jl_call2 <- function(f, a) {
    .Call(C_juliaapi_call2, f, a, b)
}


#' @export
jl_call3 <- function(str) {
    .Call(C_juliaapi_call3, f, a, b, c)
}
