#include "ntt.h"
#include <stdio.h>
#include "../headers/symbols.h"
#include "../headers/operations.h"
#include "../headers/utils.h"

#define MAX_DIGITS 1000001
#define RESULT_SIZE 2000002


int main() {
    static char num1_str[MAX_DIGITS];
    static char num2_str[MAX_DIGITS];
    static long long result_ll[RESULT_SIZE];
    static int final_result[RESULT_SIZE];
    scanf("%s", num1_str);
    scanf("%s", num2_str);
    
    static int num1_int[MAX_DIGITS];
    static int num2_int[MAX_DIGITS];
    int len1 = parse_big_int(num1_str, num1_int);
    int len2 = parse_big_int(num2_str, num2_int);

    multiply_ntt(result_ll, num1_int, len1, num2_int, len2);

    long long carry = 0;
    int i = 0;
    int result_len = add(len1, len2);
    normalize_loop:
        if (GREATER_OR_EQUAL(i, result_len)) goto normalize_end;
        long long current_val = add(result_ll[i], carry);
        final_result[i] = mod(current_val, 10);
        carry = div(current_val, 10);
        i = add(i, 1);
        goto normalize_loop;
    normalize_end:;

    // sisa carry
    carry_loop:
        if (LESSER_OR_EQUAL(carry, 0)) goto carry_end;
        final_result[i] = mod(carry, 10);
        carry = div(carry, 10);
        i = add(i, 1);
        goto carry_loop;
    carry_end:;

    print_bignum(final_result, add(result_len, 5));

    return 0;
}