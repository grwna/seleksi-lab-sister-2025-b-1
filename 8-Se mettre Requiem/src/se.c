#include "symbols.h"
#include "utils.h"
#include "operations.h"

void big_num_mult(int res[], int num1[], int len1, int num2[], int len2){
    int i = 0;
    outer:
        int j = 0;
        inner:
            uint product = mult(num1[j], num2[i]);
            uint res_idx = (uint)add(i,j);
            
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
            res[i] = current_val;
            res[add(i,1)] = add(res[add(i,1)], carry);
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