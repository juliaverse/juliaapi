#' @name juliaapi
#' @docType package
#' @useDynLib juliaapi
#' @import Rcpp
NULL

JL_CONSTS <- new.env(parent = emptyenv())

#' @export
jl_constants <- function(which) {
    JL_CONSTS[[which]]
}

#' @export
jl_init <- function() {
    libjulia <- system2(
        "julia", shQuote(c("-e", 'print(Libdl.dlpath("libjulia"))')), stdout = TRUE)
    juliaapi_init(libjulia)
    juliaapi_load_constants(JL_CONSTS)
    invisible(TRUE)
}

#' @export
print.jl_value <- function(s) {
    juliaapi_print(s)
}
