#ifndef PLATFORM_WIN32_H
#define PLATFORM_WIN32_H

#ifdef _WIN32

#define PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

#define STRUCT_ALIGN_TIME 32
#define START_END_DEFAULT ({0})

#define T(x) L##x
#define print_out(fmt, ...) (print_windows(fmt, __VA_ARGS__))

typedef LARGE_INTEGER TimeType;

void print_windows(const unsigned short *fmt, ...);

#endif
#endif
