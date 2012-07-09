#ifndef __PYX_HAVE__pybg__instruments__fixedfloatswap
#define __PYX_HAVE__pybg__instruments__fixedfloatswap


/* "pybg/instruments/fixedfloatswap.pyx":37
 * cimport pybg._curves as _curves
 * 
 * cdef public enum SwapPayType:             # <<<<<<<<<<<<<<
 *     FixedPayer    = _fixedfloatswap.FixedPayer
 *     FixedReceiver = _fixedfloatswap.FixedReceiver
 */
enum SwapPayType {

  /* "pybg/instruments/fixedfloatswap.pyx":39
 * cdef public enum SwapPayType:
 *     FixedPayer    = _fixedfloatswap.FixedPayer
 *     FixedReceiver = _fixedfloatswap.FixedReceiver             # <<<<<<<<<<<<<<
 * 
 * 
 */
  FixedPayer = bondgeek::FixedPayer,
  FixedReceiver = bondgeek::FixedReceiver
};

#ifndef __PYX_HAVE_API__pybg__instruments__fixedfloatswap

#ifndef __PYX_EXTERN_C
  #ifdef __cplusplus
    #define __PYX_EXTERN_C extern "C"
  #else
    #define __PYX_EXTERN_C extern
  #endif
#endif

#endif /* !__PYX_HAVE_API__pybg__instruments__fixedfloatswap */

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initfixedfloatswap(void);
#else
PyMODINIT_FUNC PyInit_fixedfloatswap(void);
#endif

#endif /* !__PYX_HAVE__pybg__instruments__fixedfloatswap */
