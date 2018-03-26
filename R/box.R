#' @export
jl_box_bool <- function(x) {
    .Call("juliaapi_box_bool", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_int8 <- function(x) {
    .Call("juliaapi_box_int8", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_uint8 <- function(x) {
    .Call("juliaapi_box_uint8", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_int16 <- function(x) {
    .Call("juliaapi_box_int16", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_uint16 <- function(x) {
    .Call("juliaapi_box_uint16", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_int32 <- function(x) {
    .Call("juliaapi_box_int32", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_uint32 <- function(x) {
    .Call("juliaapi_box_uint32", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_char <- function(x) {
    .Call("juliaapi_box_char", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_int64 <- function(x) {
    .Call("juliaapi_box_int64", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_uint64 <- function(x) {
    .Call("juliaapi_box_uint64", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_float32 <- function(x) {
    .Call("juliaapi_box_float32", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_float64 <- function(x) {
    .Call("juliaapi_box_float64", PACKAGE = "juliaapi", x)
}

#' @export
jl_box_voidpointer <- function(x) {
    .Call("juliaapi_box_voidpointer", PACKAGE = "juliaapi", x)
}
