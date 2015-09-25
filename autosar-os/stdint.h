#ifndef STDINT_H_
#define STDINT_H_

#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed long int32_t;
typedef unsigned long uint32_t;
typedef signed long long int64_t;
typedef unsigned long long uint64_t;

#ifndef NULL
#define NULL 0
#endif

extern int vsnprintf(char* msg, size_t size, const char* format, va_list args);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* STDINT_H_ */
