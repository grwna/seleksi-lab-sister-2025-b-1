#include "../headers/utils.h"

uint string_len(char* str) {
    uint length = 0;
    char* ptr = str;

    loop_start:
        if (EQUAL(*ptr,0)) goto loop_end;
        length = add(length, 1);
        ptr = (char*)((ull) add((ull) ptr, 1));
    goto loop_start;

    loop_end:
        return length;
}

uint parse_big_int(char string[], int num[]) {
    uint length = string_len(string);
    uint string_idx = 0;
    uint num_idx = 0;

    string_idx = sub((ull)length, 1);   // skip null
    loop_start:
        if (GREATER(num_idx, sub(length,1))) goto loop_end;

        char digit_char = string[string_idx];
        int digit_val = sub(digit_char, '0');
        num[num_idx] = digit_val;
        if (EQUAL(string_idx, 0)) goto loop_end;
        num_idx = add((ull)num_idx, 1);        
        string_idx = (uint)sub((ull)string_idx, 1);
    
    goto loop_start;

    loop_end:
        return length;
}

void print_bignum(int num[], uint size) {
    int i;
    uint start_index = add(size, 1);

    i = sub(size, 1);
    find_length:
        if (NOT_EQUAL(num[i], 0)) {
            start_index = i;
            goto found;
        }
        if (EQUAL(i, 0)) {
            start_index = add(size, 1);
            goto found;
        }
        i = sub(i, 1);
        goto find_length;

    found:
        if (EQUAL(start_index,add(size, 1))) {
            printf("0");
            goto end_print;
        }
        i = start_index;

    print_loop:
        printf("%d", num[i]);
        if (EQUAL(i, 0)) goto end_print;
        i = sub(i, 1);
        goto print_loop;

    end_print:
        printf("\n");
}