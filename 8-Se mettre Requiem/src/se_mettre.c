#include "headers/utils.h"

// Uses MACRO indexing

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

    string_idx = sub((unsigned long long)length, 1);   // skip null
    loop_start:
        if (GREATER(num_idx, sub(length,1))) goto loop_end;

        char digit_char = INDEX(string, string_idx, char, 1);
        int digit_val = sub(digit_char, '0');
        INDEX(num, num_idx, int, 4) = digit_val;
        if (EQUAL(string_idx, 0)) goto loop_end;
        num_idx = add((unsigned long long)num_idx, 1);        
        string_idx = (unsigned int)sub((unsigned long long)string_idx, 1);
    
    goto loop_start;

    loop_end:
        return length;
}

void print_bignum(int num[], unsigned int size) {
    int i;
    unsigned int start_index = add(size, 1);

    i = sub(size, 1);
    find_length:
        if (NOT_EQUAL(INDEX(num, i, int, 4), 0)) {
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
        printf("%d", INDEX(num, i, int, 4));
        if (EQUAL(i, 0)) goto end_print;
        i = sub(i, 1);
        goto print_loop;

    end_print:
        printf("\n");
}

void big_num_mult(int res[], int num1[], int len1, int num2[], int len2){
    int i = 0;
    outer:
        int j = 0;
        inner:
            unsigned int product = mult(INDEX(num1, j, int, 4), INDEX(num2, i, int, 4));
            unsigned int res_idx = (unsigned int)add(i,j);
            
            INDEX(res, res_idx, int, 4) = add(INDEX(res, res_idx, int, 4), product);
            j = add(j, 1);
            if (GREATER(len1, j)) goto inner;
        i = add(i, 1);
        if (GREATER(len2, i)) goto outer;
    i = 0;
    normalize:
        if (GREATER(INDEX(res, i, int, 4), 9)) {
            int carry = 0;
            int current_val = INDEX(res, i, int, 4);

            // optimasi normalisasi
            div_10000:
                if (GREATER(current_val, 9999)) {
                    current_val = sub(current_val, 10000);
                    carry = add(carry, 1000);
                    goto div_10000;
                }
            div_1000:
                if (GREATER(current_val, 999)) {
                    current_val = sub(current_val, 1000);
                    carry = add(carry, 100);
                    goto div_1000;
                }
            div_100:
                if (GREATER(current_val, 99)) {
                    current_val = sub(current_val, 100);
                    carry = add(carry, 10);
                    goto div_100;
                }
            div_10:
                if (GREATER(current_val, 9)) {
                    current_val = sub(current_val, 10);
                    carry = add(carry, 1);
                    goto div_10;
                }
            INDEX(res, i, int, 4) = current_val;
            INDEX(res, add(i, 1), int, 4) = add(INDEX(res, add(i, 1), int, 4), carry);
            }
        i = add(i, 1);
        // if (GREATER(sub(add(len1, len2), 1), i)) goto normalize;
        if (GREATER(add(len1, len2), i)) goto normalize;
}

// MAX SIZES
#define NUMSIZE  1001
#define NUMSTRINGSIZE add(NUMSIZE, 1) 
#define RESULT_SIZE mult(NUMSIZE, 2) 

int main(){
    char num1[NUMSTRINGSIZE];
    char num2[NUMSTRINGSIZE];
    scanf("%s", num1);
    scanf("%s", num2);
    
    int int_num1[NUMSIZE];
    int int_num2[NUMSIZE];

    uint len1 = parse_big_int(num1, int_num1);
    uint len2 = parse_big_int(num2, int_num2);

    int result[RESULT_SIZE];

    big_num_mult(result, int_num1, len1, int_num2, len2);
    print_bignum(result, RESULT_SIZE);
    return 0;
}