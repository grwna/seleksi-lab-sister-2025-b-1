#include <stdio.h>
#include "../headers/ntt.h"
#include "../headers/symbols.h"
#include "../headers/operations.h"
#include "../headers/utils.h"

#define MAX_DIGITS 1000001
#define RESULT_SIZE 2000002


int main() {
    static char num1_str[MAX_DIGITS];
    static char num2_str[MAX_DIGITS];
    static ull result_ull[RESULT_SIZE];
    static int final_result[RESULT_SIZE];
    scanf("%s", num1_str);
    scanf("%s", num2_str);
    
    int num1_int[MAX_DIGITS];
    int num2_int[MAX_DIGITS];
    int len1 = parse_big_int(num1_str, num1_int);
    int len2 = parse_big_int(num2_str, num2_int);

    multiply_ntt(result_ull, num1_int, len1, num2_int, len2);

    ull carry = 0;
    int i = 0;
    int result_len = add(len1, len2);
    normalize_loop:
        if (GREATER_OR_EQUAL(i, result_len)) goto normalize_end;
        ull current_val = add(INDEX(result_ull, i, ull, ULL), carry);
        INDEX(final_result, i, int, INT) = mod(current_val, 10);
        carry = div(current_val, 10);
        i = add(i, 1);
        goto normalize_loop;
    normalize_end:;

    // sisa carry
    carry_loop:
        if (LESSER_OR_EQUAL(carry, 0)) goto carry_end;
        INDEX(final_result, i, int, INT) = mod(carry, 10);
        carry = div(carry, 10);
        i = add(i, 1);
        goto carry_loop;
    carry_end:;

    print_bignum(final_result, add(result_len, 2));

    return 0;
}