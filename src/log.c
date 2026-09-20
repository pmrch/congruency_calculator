#include "log.h"
#include <stdio.h>

void log_internal(str_ref LEVEL, str_ref COLOR, str_ref SRCFILE, int line, str_ref FUNCTION, const char *fmt, ...) {
    va_list args;
    (void)fprintf(stderr, "%s[%s]\x1b[0m %s:%d:%s: ", COLOR, LEVEL, SRCFILE, line, FUNCTION);

    va_start(args, fmt);
    (void)vfprintf(stderr, fmt, args);
    va_end(args);

    (void)fprintf(stderr, "\n");
}
