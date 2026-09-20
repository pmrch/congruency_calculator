#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "utils.h"

static const char *const VALS[3] = {"a=", "b=", "m="};

void populate_data(InMatrix inputs, Numbers *all_nums) {
    size_t        out_nums[INPUTS] = {0};
    unsigned long number           = 0;

    // clang-format off
    #pragma unroll
    for (uint8_t i = 0; i < 3; i++) {
        printf("%s", VALS[i]);
        if (fgets(inputs[i], INPUT_MAX, stdin) == NULL) {
            LOG_ERROR("Failed to read line for iteration: %u", i);
            continue;
        }

        inputs[i][strlen(inputs[i]) - 1] = '\0';
        number = strtoul(inputs[i], NULL, BASE);
        out_nums[i] = number;
    }

    all_nums->num_a = out_nums[0];
    all_nums->num_b = out_nums[1];
    all_nums->mod = out_nums[2];
}
// clang-format on

size_t calculate_gcd(size_t num1, size_t num2) {
    size_t remainder = 1;
    size_t quotient  = 1;

    size_t last_coeff  = num1 > num2 ? num1 : num2;
    size_t other_coeff = num1 == last_coeff ? num2 : num1;

    // NOLINTNEXTLINE(altera-unroll-loops)
    while (remainder != 0) {
        quotient  = last_coeff / other_coeff;
        remainder = last_coeff - (quotient * other_coeff);

        last_coeff  = other_coeff;
        other_coeff = remainder;
    }

    return last_coeff > 1 ? last_coeff : 1;
}

void simplify_eq(Numbers *nums) {
    if (nums->num_a % nums->gcd == 0 && nums->num_b % nums->gcd == 0 && nums->mod % nums->gcd == 0) {
        nums->num_a = nums->num_a / nums->gcd;
        nums->num_b = nums->num_b / nums->gcd;
        nums->mod   = nums->mod / nums->gcd;
    }

    // print_out(L"Simplified expression: %zux \u2261 %zu (mod %zu)\n", nums->num_a, nums->num_b, nums->mod);
}

size_t get_inverse_mod(Numbers *nums) {
    size_t inverse_mod = 1;
    size_t remainder   = 0;
    size_t numerator   = 0;

    // NOLINTNEXTLINE(altera-unroll-loops)
    while (true) {
        numerator = (nums->mod * inverse_mod) + 1;
        // LOG_INFO("calc: (%zu)%zu + %zu = %zu", nums->mod, inverse_mod, nums->num_b, numerator);
        remainder = (numerator % nums->num_a);

        if (remainder == 0) {
            inverse_mod = numerator / nums->num_a;
            // LOG_INFO("%zu %c %zu = %zu", numerator, '%', nums->num_a, remainder);
            break;
        }

        inverse_mod++;
    }

    return inverse_mod;
}

size_t get_solutions(Numbers *nums, size_t inverse_mod, size_t results[]) {
    size_t num_b = nums->num_b * inverse_mod;
    size_t x_1   = num_b % nums->mod;
    results[0]   = x_1;

    for (size_t i = 1; i < nums->gcd; i++) { results[i] = results[i - 1] + nums->mod; }
    return 0;
}
