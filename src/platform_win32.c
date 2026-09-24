#ifdef _WIN32

#include "log.h"
#include "platform.h"
#include "utils.h"

#include <stdarg.h>
#include <stdlib.h>

void print_windows(const unsigned short *fmt, ...) {
    va_list va_args;
    long    chars_written = 0;
    DWORD   written       = 0;

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    // const unsigned short *text = L"Got %zu \u2261 %zu (mod %zu)";
    unsigned short textbuf[BUF_DEF] = {0};

    va_start(va_args, fmt);
    if (fmt == NULL) { return; }
    chars_written = vswprintf_s(textbuf, BUF_DEF, fmt, va_args);

    if (chars_written <= 0) {
        LOG_ERROR("%s", "Failed to write va_args to text buffer!");
        return;
    }

    WriteConsoleW(handle, textbuf, (DWORD)chars_written, &written, NULL);
    va_end(va_args);
}


void start_timer(TimerData *tdata) {
    QueryPerformanceFrequency((TimeType *)(tdata->extra));
    QueryPerformanceCounter(&tdata->start);
}

const char *end_timer(TimerData *tdata) {
    double total = 0.0;
    void *extra = tdata->extra;
    TimeType *dextra = (TimeType *)(extra);

    QueryPerformanceCounter(&tdata->end);

    total       = (double)(tdata->end.QuadPart - tdata->start.QuadPart) / (double)(dextra->QuadPart);
    tdata->diff = (long)total;

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

//error C2040: 'end_timer': 'void (TimerData *)' differs in levels of indirection from 'const char *(TimerData *)'