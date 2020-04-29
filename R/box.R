#' @export
jl_box_bool <- function(x) {
    .Call(C_juliaapi_box_bool, x)
}

#' @export
jl_box_int8 <- function(x) {
    .Call(C_juliaapi_box_int8, x)
}

#' @export
jl_box_uint8 <- function(x) {
    .Call(C_juliaapi_box_uint8, x)
}

#' @export
jl_box_int16 <- function(x) {
    .Call(C_juliaapi_box_int16, x)
}

#' @export
jl_box_uint16 <- function(x) {
    .Call(C_juliaapi_box_uint16, x)
}

#' @export
jl_box_int32 <- function(x) {
    .Call(C_juliaapi_box_int32, x)
}

#' @export
jl_box_uint32 <- function(x) {
    .Call(C_juliaapi_box_uint32, x)
}

#' @export
jl_box_char <- function(x) {
    .Call(C_juliaapi_box_char, x)
}

#' @export
jl_box_int64 <- function(x) {
    .Call(C_juliaapi_box_int64, x)
}

#' @export
jl_box_uint64 <- function(x) {
    .Call(C_juliaapi_box_uint64, x)
}

#' @export
jl_box_float32 <- function(x) {
    .Call(C_juliaapi_box_float32, x)
}

#' @export
jl_box_float64 <- function(x) {
    .Call(C_juliaapi_box_float64, x)
}

#' @export
jl_box_voidpointer <- function(x) {
    .Call(C_juliaapi_box_voidpointer, x)
}
