#' @export
jl_unbox_bool <- function(x) {
    .Call("juliaapi_unbox_bool", PACKAGE = "juliaapi", x)
}

#' @export
jl_unbox_int8 <- function(x) {
    .Call("juliaapi_unbox_int8", PACKAGE = "juliaapi", x)
}

#' @export
jl_unbox_uint8 <- function(x) {
    .Call("juliaapi_unbox_uint8", PACKAGE = "juliaapi", x)
}

#' @export
jl_unbox_int16 <- function(x) {
    .Call("juliaapi_unbox_int16", PACKAGE = "juliaapi", x)
}

#' @export
jl_unbox_uint16 <- function(x) {
    .Call("juliaapi_unbox_uint16", PACKAGE = "juliaapi", x)
}

#' @export
jl_unbox_int32 <- function(x) {
    .Call("juliaapi_unbox_int32", PACKAGE = "juliaapi", x)
}

#' @export
jl_unbox_uint32 <- function(x) {
    .Call("juliaapi_unbox_uint32", PACKAGE = "juliaapi", x)
}

#' @export
jl_unbox_int64 <- function(x) {
    .Call("juliaapi_unbox_int64", PACKAGE = "juliaapi", x)
}

#' @export
jl_unbox_uint64 <- function(x) {
    .Call("juliaapi_unbox_uint64", PACKAGE = "juliaapi", x)
}

#' @export
jl_unbox_float32 <- function(x) {
    .Call("juliaapi_unbox_float32", PACKAGE = "juliaapi", x)
}

#' @export
jl_unbox_float64 <- function(x) {
    .Call("juliaapi_unbox_float64", PACKAGE = "juliaapi", x)
}

#' @export
jl_unbox_voidpointer <- function(x) {
    .Call("juliaapi_unbox_voidpointer", PACKAGE = "juliaapi", x)
}
