#ifndef _WIN32
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>

#define ONE_BILLION 1000000000L
#define ONE_MILLION 1000000L
#define ONE_THOUSAND 1000L

__attribute__((format(printf, 1, 2))) void print_linux(const char *fmt, ...) {
    if (fmt == NULL) { return; }

    va_list args;
    va_start(args, fmt);
    (void)vfprintf(stdout, fmt, args);
    (void)fprintf(stdout, "\n");

    va_end(args);
}

void start_timer(TimerData *tdata) {
    if (tdata == NULL) { return; }
    clock_gettime(CLOCK_MONOTONIC, &tdata->start);
}

const char *end_timer(TimerData *tdata) {
    clock_gettime(CLOCK_MONOTONIC, &tdata->end);

    uint64_t seconds     = (uint64_t)(tdata->end.tv_sec - tdata->start.tv_sec);
    uint64_t nanoseconds = (uint64_t)(tdata->end.tv_nsec - tdata->start.tv_nsec);

    // Convert total elapsed time into nanoseconds (or seconds/milliseconds)
    tdata->diff = (seconds * ONE_BILLION) + nanoseconds;

    if (tdata->diff / ONE_BILLION >= 1) {
        tdata->diff /= ONE_BILLION;
        return "s";
    }

    if (tdata->diff / ONE_MILLION >= 1) {
        tdata->diff /= ONE_MILLION;
        return "ms";
    }

    if (tdata->diff / ONE_THOUSAND >= 1) {
        tdata->diff /= ONE_THOUSAND;
        return "us";
    }

    return "ns";
}

#endif
