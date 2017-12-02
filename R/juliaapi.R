#' @name juliaapi
#' @docType package
#' @useDynLib juliaapi
#' @import Rcpp
NULL

#' @export
jl_init <- function() {
    libjulia <- system2(
        "julia", shQuote(c("-e", 'print(Libdl.dlpath("libjulia"))')), stdout = TRUE)
    juliaapi_init(libjulia)
}

#' @export
jl_eval <- function(string) {
    ret <- juliaapi_eval_string(string)
    class(ret) <- "jl_value"
    ret
}

#' @export
print.jl_value <- function(s) {
    juliaapi_print(s)
}
