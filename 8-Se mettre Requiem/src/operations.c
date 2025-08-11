#include "headers/operations.h"

ull add(ull a, ull b){
    loop:
        ull carry = a & b;
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

ll barrett_reduce(ll val, ll prime, unsigned __int128 mu) {
    unsigned __int128 q = (mult(val, mu)) >> 62; 
    ll r = sub(val, mult(q, prime));
    // Hasilnya bisa sedikit lebih besar dari prime, jadi perlu koreksi akhir
    if (GREATER_OR_EQUAL(r, prime)) r = sub(r, prime);
    // if (GREATER_OR_EQUAL(r, prime)) r = sub(r, prime); // Koreksi kedua (jarang terjadi tapi aman)
    return r;
}