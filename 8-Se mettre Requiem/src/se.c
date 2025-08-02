#include "utils.h"

// Uses Native Indexing, way faster

unsigned int string_len(const char* str) {
    unsigned int length = 0;
    const char* ptr = str;

    loop_start:
        if (EQUAL(*ptr,0)) goto loop_end;
        length = add(length, 1);
        ptr = (char*)((unsigned long long) add((unsigned long long) ptr, 1));
    goto loop_start;

    loop_end:
        return length;
}

unsigned int parse_big_int(char string[], int num[]) {
    unsigned int length = string_len(string);
    unsigned int string_idx = 0;
    unsigned int num_idx = 0;

    string_idx = subtract((unsigned long long)length, 1);   // skip null
    loop_start:
        if (GREATER(num_idx, subtract(length,1))) goto loop_end;

        char digit_char = string[string_idx];
        int digit_val = subtract(digit_char, '0');
        num[num_idx] = digit_val;
        if (EQUAL(string_idx, 0)) goto loop_end;
        num_idx = add((unsigned long long)num_idx, 1);        
        string_idx = (unsigned int)subtract((unsigned long long)string_idx, 1);
    
    goto loop_start;

    loop_end:
        return length;
}

void print_bignum(int num[], unsigned int size) {
    int i;
    unsigned int start_index = add(size, 1);

    i = subtract(size, 1);
    find_length:
        if (NOT_EQUAL(num[i], 0)) {
            start_index = i;
            goto found;
        }
        if (EQUAL(i, 0)) {
            start_index = add(size, 1);
            goto found;
        }
        i = subtract(i, 1);
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
        i = subtract(i, 1);
        goto print_loop;

    end_print:
        printf("\n");
}

void big_num_multiply(int res[], int num1[], int len1, int num2[], int len2){
    int i = 0;
    outer:
        int j = 0;
        inner:
            unsigned int product = multiply(num1[j], num2[i]);
            unsigned int res_idx = (unsigned int)add(i,j);
            
            res[res_idx] = add(res[res_idx], product);
            j = add(j, 1);
            if (GREATER(len1, j)) goto inner;
        i = add(i, 1);
        if (GREATER(len2, i)) goto outer;
    i = 0;
    normalize:
        if (GREATER(res[i], 9)) {
            int carry = 0;
            int current_val = res[i];
            div_10000:
                if (GREATER(current_val, 9999)) {
                    current_val = subtract(current_val, 10000);
                    carry = add(carry, 1000);
                    goto div_10000;
                }
            div_1000:
                if (GREATER(current_val, 999)) {
                    current_val = subtract(current_val, 1000);
                    carry = add(carry, 100);
                    goto div_1000;
                }
            div_100:
                if (GREATER(current_val, 99)) {
                    current_val = subtract(current_val, 100);
                    carry = add(carry, 10);
                    goto div_100;
                }
            div_10:
                if (GREATER(current_val, 9)) {
                    current_val = subtract(current_val, 10);
                    carry = add(carry, 1);
                    goto div_10;
                }
            res[i] = current_val;
            res[add(i, 1)] = add(res[add(i,1)], carry);
            }
        i = add(i, 1);
        if (GREATER(subtract(add(len1, len2), 1), i)) goto normalize;
}

int main(){
    char num1[1001];
    char num2[1001];
    scanf("%s", num1);
    scanf("%s", num2);
    
    int int_num1[1000];
    int int_num2[1000];

    unsigned int len1 = parse_big_int(num1, int_num1);
    unsigned int len2 = parse_big_int(num2, int_num2);

    int result[2000];

    big_num_multiply(result, int_num1, len1, int_num2, len2);
    print_bignum(result, 2000);
    return 0;
}