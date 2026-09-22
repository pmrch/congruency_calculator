#ifdef _WIN32

#include "log.h"
#include "platform.h"
#include "utils.h"

#include <stdarg.h>
#include <stdlib.h>

void start_timer(TimerData *tdata) {
    QueryPerformanceFrequency((TimeType *)(tdata->extra));
    QueryPerformanceCounter(&tdata->start);
}

void end_timer(TimerData *tdata) {
    double total = 0.0;
    QueryPerformanceCounter(&tdata->end);

    total       = (double)(tdata->end.QuadPart - tdata->start.QuadPart) / (double)((TimeType *)tdata->extra)->QuadPart;
    tdata->diff = (long)total;
}

#endif
