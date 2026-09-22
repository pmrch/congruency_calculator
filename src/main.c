#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "platform.h"
#include "utils.h"

#define OLEN BUF_DEF / 4

int main(void) {
    TimerData tdata = {.extra = malloc(sizeof(void *)), .end = START_END_DEFAULT, .start = START_END_DEFAULT, .diff = 0};

    Numbers  all_nums = {.gcd = 0, .num_a = 0, .num_b = 0, .mod = 0};
    InMatrix inputs   = {{0}, {0}};

    char  outbuf[OLEN] = {0};
    long  ptr_add_len  = 0;
    char *buf_ptr      = outbuf;

    int64_t  inverse_mod = 0;
    int64_t *results     = NULL;

    print_out(T("%s"), T("Consider the following format: a \u2261 b (mod m)"));
    populate_data(inputs, &all_nums);
    start_timer(&tdata);

    all_nums.gcd = calculate_gcd(all_nums.num_a, all_nums.mod);
    print_out(T("\nGot: %lix \u2261 %li (mod %li)"), all_nums.num_a, all_nums.num_b, all_nums.mod);
    // print_out(L"GCD (%li, %li) = %li\n", all_nums.num_a, all_nums.mod, all_nums.gcd);

    if (all_nums.num_b % all_nums.gcd != 0) {
        printf("%li is not cleanly divisible with %li, hence, no congruent solutions!\n", all_nums.num_b, all_nums.gcd);
        return 0;
    }

    // 84x ≡ 180 (mod 756)
    //  Simplify expression if possible
    simplify_eq(&all_nums);
    inverse_mod = get_inverse_mod(&all_nums);
    // print_out(L"Got inverse mod: %li\n", inverse_mod);

    results = (int64_t *)malloc((size_t)all_nums.gcd * sizeof(size_t));
    if (results == NULL) {
        LOG_ERROR("%s", "Failed to allocate memory for results");
        return -1;
    }

    get_solutions(&all_nums, inverse_mod, results);

#pragma unroll 5
    for (int64_t i = 0; i < all_nums.gcd; ++i) {
        size_t buflen = sizeof(outbuf) - (size_t)ptr_add_len;
        ptr_add_len += snprintf(outbuf + ptr_add_len, buflen, "%sx_%li = %li", i ? ", " : "", i, results[i]);
    }

    const char *const UNIT = end_timer(&tdata);
    printf("Got results: %s\n", buf_ptr);
    printf("total runtime: %lu %s\n", tdata.diff, UNIT);

    free(results);
    return 0;
}
