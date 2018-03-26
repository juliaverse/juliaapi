#' @export
jl_init <- function(libpath = NULL) {
    if (is.null(libpath)) {
        libpath <- system2(
                "julia", shQuote(c("-e", 'print(Libdl.dlpath("libjulia"))')), stdout = TRUE)
    }
    .Call("juliaapi_initialize", PACKAGE = "juliaapi", libpath)

    pkg_env <- asNamespace("juliaapi")
    for (objname in ls(envir = pkg_env)) {
        obj <- get(objname, envir = pkg_env)
        if (typeof(obj) == "externalptr" && class(obj) == "jl_value") {
            eval(substitute(set_jl_value_xptr(x), list(x = as.name(objname))))
        }
    }

    invisible(NULL)
}

#' @export
jl_is_initialized <- function() {
    .Call("juliaapi_is_initialized", PACKAGE = "juliaapi")
}
