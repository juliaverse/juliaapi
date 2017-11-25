// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// juliaapi_init
bool juliaapi_init(const std::string& libpath);
RcppExport SEXP _juliaapi_juliaapi_init(SEXP libpathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type libpath(libpathSEXP);
    rcpp_result_gen = Rcpp::wrap(juliaapi_init(libpath));
    return rcpp_result_gen;
END_RCPP
}
// juliaapi_eval_string
SEXP juliaapi_eval_string(const char* str, bool preserve);
RcppExport SEXP _juliaapi_juliaapi_eval_string(SEXP strSEXP, SEXP preserveSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const char* >::type str(strSEXP);
    Rcpp::traits::input_parameter< bool >::type preserve(preserveSEXP);
    rcpp_result_gen = Rcpp::wrap(juliaapi_eval_string(str, preserve));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_juliaapi_juliaapi_init", (DL_FUNC) &_juliaapi_juliaapi_init, 1},
    {"_juliaapi_juliaapi_eval_string", (DL_FUNC) &_juliaapi_juliaapi_eval_string, 2},
    {NULL, NULL, 0}
};

RcppExport void R_init_juliaapi(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
