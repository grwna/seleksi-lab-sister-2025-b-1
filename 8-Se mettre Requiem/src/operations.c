#include "headers/operations.h"

ull add(ull a, ull b){
    ull carry;
    loop:
        carry = a & b;
        a = a ^ b;
        b = carry << 1;
    if (NOT_EQUAL(b, 0)) goto loop;
    return a;
}

ull mult(ull a, ull b){
    ull result = 0;
    loop:
        if (b & 1) result = add(result, a);
        a = a << 1;
        b = b >> 1;
    if (NOT_EQUAL(b, 0)) goto loop;
    return result;
}

ull sub(ull a, ull b) {
    ull neg_b = add(~b, 1);
    return add(a, neg_b);
}

ull div(ull a, ull b){
    ull quotient = 0;
    ull remainder = 0;

    int bit = 63;  // bit position

    loop_start:
        remainder = remainder << 1;
        remainder = remainder | ((a >> bit) & 1);

        ull temp = sub(remainder, b);

        if (EQUAL(temp >> 63, 1)){
            quotient = quotient << 1;
            goto iteration_end;
        }
        remainder = temp;
        quotient = quotient << 1;
        quotient = quotient | 1;
            
    iteration_end:
    bit = sub(bit, 1);
    if (GREATER_OR_EQUAL(bit, 0)) goto loop_start;

    return quotient;
}

ull mod(ull a, ull b) {
    if (EQUAL(b, 0)) {
        return a; 
    }

    ull q = div(a, b);
    ull product = mult(q, b);
    ull remainder = sub(a, product);

    return remainder;
}

ull mult_high(ull a, ull b) {
    ull a_lo = a & 0xFFFFFFFF;
    ull a_hi = a >> 32;
    ull b_lo = b & 0xFFFFFFFF;
    ull b_hi = b >> 32;

    ull p00 = mult(a_lo, b_lo);
    ull p01 = mult(a_lo, b_hi);
    ull p10 = mult(a_hi, b_lo);
    ull p11 = mult(a_hi, b_hi);

    ull p00_hi = p00 >> 32;
    ull temp = add(p10, p00_hi);
    ull mid_sum = add(temp, p01);

    ull high_part = add(p11, (mid_sum >> 32));
    
    if (mid_sum < temp) {
        high_part = add(high_part, (1ULL << 32));
    }
    
    return high_part;
}

ull barrett_reduce(ull x, ull prime, ull m) {
    ull q = mult_high(x, m);
    ull r = sub(x, mult(q, prime));
    ull difference = sub(r, prime);
    if (EQUAL(((difference >> 63) & 1), 0)) {
        r = difference;
    }

    return r;
}
