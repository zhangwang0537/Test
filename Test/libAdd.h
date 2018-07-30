//
// MATLAB Compiler: 6.5 (R2017b)
// Date: Sat Jul 28 21:58:55 2018
// Arguments: "-B""macro_default""-W""cpplib:libAdd""-T""link:lib""Add"
//

#ifndef __libAdd_h
#define __libAdd_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libAdd_C_API 
#define LIB_libAdd_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_libAdd_C_API 
bool MW_CALL_CONV libAddInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_libAdd_C_API 
bool MW_CALL_CONV libAddInitialize(void);

extern LIB_libAdd_C_API 
void MW_CALL_CONV libAddTerminate(void);

extern LIB_libAdd_C_API 
void MW_CALL_CONV libAddPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_libAdd_C_API 
bool MW_CALL_CONV mlxAdd(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_libAdd
#define PUBLIC_libAdd_CPP_API __declspec(dllexport)
#else
#define PUBLIC_libAdd_CPP_API __declspec(dllimport)
#endif

#define LIB_libAdd_CPP_API PUBLIC_libAdd_CPP_API

#else

#if !defined(LIB_libAdd_CPP_API)
#if defined(LIB_libAdd_C_API)
#define LIB_libAdd_CPP_API LIB_libAdd_C_API
#else
#define LIB_libAdd_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_libAdd_CPP_API void MW_CALL_CONV Add(int nargout, mwArray& C, const mwArray& A, const mwArray& B);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
