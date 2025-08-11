#include "../headers/symbols.h"

int GREATER(int a, int b) {
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

int GREATER_OR_EQUAL(int a, int b){
    return GREATER(a, sub(b,1));
}

int LESSER(int a, int b){
    return GREATER(b, a);
}

int LESSER_OR_EQUAL(int a, int b){
    return LESSER(a, add(b,1));
}

int LESSER_ULL(ull a, ull b) {
    return (sub(a, b) >> 63) & 1;
}

