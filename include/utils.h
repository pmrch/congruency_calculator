#ifndef UTILS_H
#define UTILS_H

#define BASE 10
#define INPUTS 3
#define INPUT_MAX 25

#define STRUCT_ALIGN16 16
#define STRUCT_ALIGN32 32

#define BUF_DEF 1024

// Windows-only code
#ifdef _WIN32
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <wchar.h>

#endif

// Define convenient type
typedef char InMatrix[INPUTS][INPUT_MAX];

typedef struct __attribute__((aligned(STRUCT_ALIGN32))) {
    size_t num_a;
    size_t num_b;
    size_t gcd;
    size_t mod;
} Numbers;

void   simplify_eq(Numbers *nums);
size_t get_inverse_mod(Numbers *nums);
size_t get_solutions(Numbers *nums, size_t inverse_mod, size_t results[]);

void   populate_data(InMatrix inputs, Numbers *all_nums);
size_t calculate_gcd(size_t num1, size_t num2);

#endif
