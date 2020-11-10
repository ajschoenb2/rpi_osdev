#ifndef INCLUDE_KERNEL_STDINT_H_
#define INCLUDE_KERNEL_STDINT_H_

typedef unsigned char uint8_t;
typedef char int8_t;

typedef unsigned short uint16_t;
typedef short int16_t;

typedef unsigned long uint32_t;
typedef long int32_t;

typedef unsigned long long uint64_t;
typedef long long int64_t;

typedef uint64_t uintptr_t;
typedef int64_t intptr_t;

typedef uint64_t ureg_t;
typedef int64_t reg_t;

typedef uint64_t size_t;
typedef int64_t ssize_t;

#endif  // INCLUDE_KERNEL_STDINT_H_
