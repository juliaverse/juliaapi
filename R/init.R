libjulia_path <- function() {
    system2(
        "julia",
        shQuote(c("-e", 'using Libdl; print(Libdl.dlpath("libjulia"))')),
        stdout = TRUE)
}

#' @export
jl_init <- function(libjulia = NULL) {
    if (is.null(libjulia)) {
        libjulia <- libjulia_path()
    }
    .Call(C_juliaapi_initialize, libjulia)

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
    .Call(C_juliaapi_is_initialized)
}
