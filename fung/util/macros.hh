#pragma once

#if defined(__UNIX__)
#define FUNG_ALWAYS_INLINE __attribute__((always_inline))
#elif defined(__WIN32__) || defined(_WIN32)
#define FUNG_ALWAYS_INLINE __forceinline
#endif
