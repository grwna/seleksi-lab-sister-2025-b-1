#include <stdio.h>

// MACROS
#define EQUAL(A, B) (!((A) ^ (B)))
#define NOT_EQUAL(A, B) (((A) ^ (B)))

// doesnt use sizeof
#define INDEX(arr_ptr, idx, elem_type, elem_size) \
    (*((elem_type *)add((unsigned long long)(arr_ptr), \
    (unsigned long long)multiply((unsigned long long)(idx), (unsigned long long)elem_size))))


// found from here: https://stackoverflow.com/questions/10096599/bitwise-operations-equivalent-of-greater-than-operator
static inline int GREATER(int a, int b) {
    int diff = a ^ b;
    diff |= diff >> 1;
    diff |= diff >> 2;
    diff |= diff >> 4;
    diff |= diff >> 8;
    diff |= diff >> 16;

    diff &= ~(diff >> 1) | 0x80000000;
    diff &= (a ^ 0x80000000) & (b ^ 0x7fffffff);

    return diff;
}

unsigned long long add(unsigned long long a, unsigned long long b){
    loop:
        unsigned long long carry = a & b;
        a = a ^ b;
        b = carry << 1;
    if (NOT_EQUAL(b, 0)) goto loop;
    return a;
}

unsigned long long multiply(unsigned long long a, unsigned long long b){
    unsigned long long result = 0;
    loop:
        if (b & 1) result = add(result, a);
        a = a << 1;
        b = b >> 1;
    if (NOT_EQUAL(b, 0)) goto loop;
    return result;
}

unsigned long long subtract(unsigned long long a, unsigned long long b) {
    unsigned long long neg_b = add(~b, 1);
    return add(a, neg_b);
}