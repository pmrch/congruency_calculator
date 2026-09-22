#ifndef PLATFORM_POSIX_H
#define PLATFORM_POSIX_H

#define PLATFORM_LINUX
#include <sys/time.h>
#include <time.h>

#define STRUCT_ALIGN_TIME 64
#define START_END_DEFAULT {0}

#define T(x) x
#define print_out(fmt, ...) (print_linux(fmt, __VA_ARGS__))
typedef struct timespec TimeType;

void print_linux(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#endif
