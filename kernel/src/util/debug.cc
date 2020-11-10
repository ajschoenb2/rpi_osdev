#include "kernel/util/debug.h"
#include "kernel/util/machine.h"
#include "kernel/util/snprintf.h"

OutputStream<char>* Debug::sink = 0;
bool Debug::debug_all = false;

void Debug::init(OutputStream<char> *sink) {
    Debug::sink = sink;
}

// static SpinLock lock;

void Debug::vprintf(const char* fmt, va_list ap) {
    if (sink) {
        // lock.lock();
        vsnprintf(*sink, 1000, fmt, ap);
        // lock.unlock();
    }
}

void Debug::printf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt,ap);
    va_end(ap);
}

void Debug::shutdown() {
    printf("| core %d requested shutdown\n", getCpu());
    printf("| system shutdown\n");
    _shutdown();
}

void Debug::vpanic(const char* fmt, va_list ap) {
    vprintf(fmt, ap);
    printf("| processor %d halting\n", getCpu());
    printf("| system shutdown\n");
    _shutdown();
}


void Debug::panic(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vpanic(fmt, ap);
    va_end(ap);
}

void Debug::vdebug(const char* fmt, va_list ap) {
    if (debug_all || flag) {
       printf("[%s] ", what);
       vprintf(fmt, ap);
       printf("\n");
    }
}

void Debug::debug(const char* fmt, ...) {
    if (debug_all || flag) {
        va_list ap;
        va_start(ap, fmt);
        vdebug(fmt, ap);
        va_end(ap);
    }
}

void Debug::missing(const char* file, int line) {
    panic("*** Missing code at %s:%d\n", file, line);
}


