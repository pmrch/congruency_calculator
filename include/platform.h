#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdarg.h>
#include <stdint.h>

#ifdef _WIN32
#include "platform_win32.h"
#else
#include "platform_posix.h"
#endif

#define STRUCT_ALIGN16 16
#define STRUCT_ALIGN32 32

#define ONE_BILLION 1000000000L
#define ONE_MILLION 1000000L
#define ONE_THOUSAND 1000L

typedef struct ALIGN_TIME {
    void    *extra;
    TimeType start;
    TimeType end;
    uint64_t diff;
} TimerData;

// Function definitions
void        start_timer(TimerData *tdata);
const char *end_timer(TimerData *tdata);

// Definitions

#endif
