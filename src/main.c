#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "platform.h"
#include "utils.h"

#define OLEN BUF_DEF / 4

int main(void) {
    TimerData tdata = {.extra = malloc(sizeof(void *)), .end = {0}, .start = {0}, .diff_ns = 0};

    Numbers  all_nums = {.gcd = 0, .num_a = 0, .num_b = 0, .mod = 0};
    InMatrix inputs   = {{0}, {0}};

    char  outbuf[OLEN] = {0};
    long  ptr_add_len  = 0;
    char *buf_ptr      = outbuf;

    size_t  inverse_mod = 0;
    size_t *results     = NULL;

    start_timer(&tdata);
    print_out(L"%s", L"Consider the following format: a \u2261 b (mod m)\n");
    populate_data(inputs, &all_nums);

    all_nums.gcd = calculate_gcd(all_nums.num_a, all_nums.mod);
    print_out(L"Got: %zux \u2261 %zu (mod %zu)\n", all_nums.num_a, all_nums.num_b, all_nums.mod);
    // print_out(L"GCD (%zu, %zu) = %zu\n", all_nums.num_a, all_nums.mod, all_nums.gcd);

    if (all_nums.num_b % all_nums.gcd != 0) {
        print_out(L"%zu is not cleanly divisible with %zu, hence, no congruent solutions!", all_nums.num_b, all_nums.gcd);
        return 0;
    }

    // Simplify expression if possible
    simplify_eq(&all_nums);
    inverse_mod = get_inverse_mod(&all_nums);
    // print_out(L"Got inverse mod: %zu\n", inverse_mod);

    results = (size_t *)malloc(all_nums.gcd * sizeof(size_t));
    if (results == NULL) {
        LOG_ERROR("%s", "Failed to allocate memory for results");
        return -1;
    }

    get_solutions(&all_nums, inverse_mod, results);
    for (size_t i = 0; i < all_nums.gcd; ++i) {
        size_t buflen = sizeof(outbuf) - (size_t)ptr_add_len;
        ptr_add_len += snprintf(outbuf + ptr_add_len, buflen, "%sx_%zu = %zu", i ? ", " : "", i, results[i]);
    }

    end_timer(&tdata);
    printf("Got results: %s | total runtime: %f ns", buf_ptr, tdata.diff_ns);

    free(results);
    return 0;
}
