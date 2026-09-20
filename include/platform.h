#ifndef PLATFORM_H
#define PLATFORM_H

#include "utils.h"

#ifdef _WIN32
#define PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#define print_out(fmt, ...) (print_windows(fmt, __VA_ARGS__))

typedef LARGE_INTEGER TimeType;

#else
#define PLATFORM_LINUX
#include <time.h>

#define print_out(fmt, ...) (print_linux(fmt, __VA_ARGS__))

typedef struct timespec TimeType;
#endif

typedef struct __attribute__((aligned(STRUCT_ALIGN32))) {
    void    *extra;
    TimeType start;
    TimeType end;
    double   diff_ns;
} TimerData;

// Function definitions
void print_windows(const unsigned short *fmt, ...);
void start_timer(TimerData *tdata);
void end_timer(TimerData *tdata);
void output_format(void);

// Definitions

#endif
