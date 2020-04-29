#' @export
jl_unbox_bool <- function(x) {
    .Call(C_juliaapi_unbox_bool, x)
}

#' @export
jl_unbox_int8 <- function(x) {
    .Call(C_juliaapi_unbox_int8, x)
}

#' @export
jl_unbox_uint8 <- function(x) {
    .Call(C_juliaapi_unbox_uint8, x)
}

#' @export
jl_unbox_int16 <- function(x) {
    .Call(C_juliaapi_unbox_int16, x)
}

#' @export
jl_unbox_uint16 <- function(x) {
    .Call(C_juliaapi_unbox_uint16, x)
}

#' @export
jl_unbox_int32 <- function(x) {
    .Call(C_juliaapi_unbox_int32, x)
}

#' @export
jl_unbox_uint32 <- function(x) {
    .Call(C_juliaapi_unbox_uint32, x)
}

#' @export
jl_unbox_int64 <- function(x) {
    .Call(C_juliaapi_unbox_int64, x)
}

#' @export
jl_unbox_uint64 <- function(x) {
    .Call(C_juliaapi_unbox_uint64, x)
}

#' @export
jl_unbox_float32 <- function(x) {
    .Call(C_juliaapi_unbox_float32, x)
}

#' @export
jl_unbox_float64 <- function(x) {
    .Call(C_juliaapi_unbox_float64, x)
}

#' @export
jl_unbox_voidpointer <- function(x) {
    .Call(C_juliaapi_unbox_voidpointer, x)
}
