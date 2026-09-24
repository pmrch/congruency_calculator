#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

typedef const char *const str_ref;

#ifdef _WIN32
#define PRINTF_LIKE(idx, fmt)
#else
#define PRINTF_LIKE(idx, fmt) __attribute__((format(printf, idx, fmt)))
#endif

#define LOG_ERROR(fmt, ...) (log_internal("ERROR", "\x1b[31m", __FILE__, __LINE__, __FUNCTION__, fmt, __VA_ARGS__))
#define LOG_INFO(fmt, ...) (log_internal("INFO", "\x1b[33m", __FILE__, __LINE__, __FUNCTION__, fmt, __VA_ARGS__))

void log_internal(str_ref LEVEL, str_ref COLOR, str_ref SRCFILE, int line, str_ref FUNCTION, char const *fmt, ...)
    PRINTF_LIKE(6, 7);

#endif
