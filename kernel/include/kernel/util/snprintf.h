#ifndef INCLUDE_KERNEL_UTIL_SNPRINTF_H_
#define INCLUDE_KERNEL_UTIL_SNPRINTF_H_

#include <stdarg.h>

#include "kernel/io/io.h"

extern "C" void snprintf(OutputStream<char>& sink, long maxlen, const char* fmt, ...);
extern "C" void vsnprintf(OutputStream<char>& sink, long maxlen, const char* fmt, va_list arg);

#endif  // INCLUDE_KERNEL_UTIL_SNPRINTF_H_
