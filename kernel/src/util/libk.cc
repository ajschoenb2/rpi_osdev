#include "kernel/util/libk.h"

#include "kernel/util/debug.h"

int isdigit(char c) {
    return (c >= '0' && c <= '9');
}

long strlen(const char* str) {
    int n = 0;
    while (*str++ != '\0') n++;
    return n;
}

extern "C" void __cxa_pure_virtual() {
    Debug::panic("__cxa_pure_virtual called\n");
}
