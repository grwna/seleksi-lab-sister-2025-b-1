#include "../headers/ntt.h"

#define MAX_N 2097152 // 2^21, cukup untuk 10^1mil

ull ntt_a[MAX_N];
ull ntt_b[MAX_N];
ull ntt_c[MAX_N];

ull power(ull a, ull b, ull modulo, ull mu) {
    ull res = 1;
    a = barrett_reduce(a, modulo, mu);
    loop:
        if (LESSER_OR_EQUAL( b, 0)) goto end;
        if (EQUAL(mod(b,2), 1)) res = barrett_reduce((mult(res, a)),  modulo, mu);
        a = barrett_reduce(mult(a,a),  modulo, mu);
        b = div(b, 2);
        goto loop;
    end:
    return res;
}

// modinv
ull inverse(ull n, ull modulo, ull mu) {
    return power(n, sub(modulo, 2), modulo, mu);
}

// iterative in place ntt
void ntt(ull* arr, int n, ull prime, ull root, ull mu, int is_inverse) {
    int i = 1;
    int j = 0;
    bit_reversal_loop:
        if (GREATER_OR_EQUAL(i, n)) goto bit_reversal_end;
        int bit = n >> 1;
        bit_reversal_inner_loop:
            if (!(j & bit)) goto bit_reversal_inner_end;
            j ^= bit;
            bit >>= 1;
            goto bit_reversal_inner_loop;
        bit_reversal_inner_end:
        j ^= bit;
        if (LESSER(i, j)) {
            ull temp = INDEX(arr, i, ull, ULL);
            INDEX(arr, i, ull, ULL) = INDEX(arr, j, ull, ULL);
            INDEX(arr, j, ull, ULL) = temp;
        }
        i = add(i, 1);
        goto bit_reversal_loop;
    bit_reversal_end:;

    // Cooley-Tukey
    int len = 2;
    main_loop:
        if (GREATER(len, n)) goto main_loop_end;
        ull w_len = power(root, div((sub(prime, 1)), len), prime, mu);
        if (is_inverse) {
            w_len = inverse(w_len, prime, mu);
        }
        i = 0;
        int div_len_2 = len >> 1;
        outer_butterfly_loop:
            if (GREATER_OR_EQUAL(i, n)) goto outer_butterfly_end;
            ull w = 1;
            j = 0;
            inner_butterfly_loop:
                int i_plus_j = add(i,j);
                int ijdivlen = add(i_plus_j, div_len_2);
                if (GREATER_OR_EQUAL(j, div_len_2)) goto inner_butterfly_end;
                ull u = INDEX(arr, i_plus_j, ull, ULL);
                ull v = barrett_reduce(mult(INDEX(arr, ijdivlen, ull, ULL), w), prime, mu);
                INDEX(arr, i_plus_j, ull, ULL) = barrett_reduce(add(u, v), prime, mu);
                INDEX(arr, ijdivlen, ull, ULL) = barrett_reduce((add(sub(u, v), prime)), prime, mu);
                w = barrett_reduce((mult(w, w_len)), prime, mu);
                j = add(j, 1);
                goto inner_butterfly_loop;
            inner_butterfly_end:
            i = add(i, len);
            goto outer_butterfly_loop;
        outer_butterfly_end:
        len <<= 1;
        goto main_loop;
    main_loop_end:;

    // scaling for inverse
    if (is_inverse) {
        ull n_inv = inverse(n, prime, mu);
        i = 0;
        scale_loop:
            if (GREATER_OR_EQUAL(i, n)) goto scale_end;
            INDEX(arr, i, ull, ULL) = barrett_reduce(mult(INDEX(arr, i, ull, ULL), n_inv), prime, mu);
            i = add(i, 1);
            goto scale_loop;
        scale_end:;
    }
}

// main ntt function
void multiply_ntt(ull res[], int num1[], int len1, int num2[], int len2) {
    // determine transformation size
    int n = 1;
    size_loop:
        if (GREATER_OR_EQUAL(n, add(len1, len2))) goto size_end;
        n <<= 1;
        goto size_loop;
    size_end:;

    ull PRIMES[] = {
        2013265921, 
        2281701377, 
        469762049
    };
    ull ROOTS[] = {
        31, 
        3, 
        3
    };
    ull MU[] = {
        9162596893, 
        8084644318, 
        39268272336
    };

    
    ull prime = sub(0,1), root = sub(0, 1);//, mu = sub(0, 1);
    ull mu = 0 ;
    int i = 0;
    ull prime_temp;
    prime_loop:
        if (GREATER_OR_EQUAL(i ,3)) goto prime_not_found; // 3 adalah jumlah prime di daftar
        prime_temp = INDEX(PRIMES, i, ull, ULL);
        if (EQUAL(mod((sub(prime_temp, 1)), n), 0)) {
            
            prime = prime_temp;
            root = INDEX(ROOTS, i, ull, ULL);
            mu = INDEX(MU, i, ull, ULL);
            // printf("%llu\n", mu);
            goto prime_end;
        }
        i = add(i, 1);
        goto prime_loop;
    prime_not_found:
        printf("Prime not found");
        return;
    prime_end:;

    // copy digit (ntt_a, ntt_b)
    i = 0;
    copy_a_loop:
        if (GREATER_OR_EQUAL(i, len1)) goto copy_a_end;
        INDEX(ntt_a, i, ull, ULL) = INDEX(num1, i, int, INT);
        i = add(i, 1);
        goto copy_a_loop;
    copy_a_end:;
    zero_pad_a_loop: // pad with zero
        if (GREATER_OR_EQUAL(i, n)) goto zero_pad_a_end;
        INDEX(ntt_a, i, ull, ULL) = 0;
        i = add(i, 1);
        goto zero_pad_a_loop;
    zero_pad_a_end:;

    i = 0;
    copy_b_loop:
        if (GREATER_OR_EQUAL(i, len2)) goto copy_b_end;
        INDEX(ntt_b, i, ull, ULL) = INDEX(num2, i, int, INT);
        i = add(i, 1);
        goto copy_b_loop;
    copy_b_end:;
    zero_pad_b_loop:
        if (GREATER_OR_EQUAL    (i, n)) goto zero_pad_b_end;
        INDEX(ntt_b, i, ull, ULL) = 0;
        i = add(i, 1);
        goto zero_pad_b_loop;
    zero_pad_b_end:;

    ntt(ntt_a, n, prime, root, mu, 0);
    ntt(ntt_b, n, prime, root, mu, 0);

    // pointwise multiplication in frequency domain
    i = 0;
    pointwise_mult_loop:
        if (GREATER_OR_EQUAL(i, n)) goto pointwise_mult_end;
        INDEX(ntt_c, i, ull, ULL) = barrett_reduce((mult(INDEX(ntt_a, i, ull, ULL), INDEX(ntt_b, i, ull, ULL))), prime, mu);
        i = add(i, 1);
        goto pointwise_mult_loop;
    pointwise_mult_end:;

    // invers NTT to return to time domain
    ntt(ntt_c, n, prime, root, mu, 1);

    i = 0;
    copy_res_loop:
        if (GREATER_OR_EQUAL(i, n)) goto copy_res_end;
        INDEX(res, i, ull, ULL) = INDEX(ntt_c, i, ull, ULL);
        i = add(i, 1);
        goto copy_res_loop;
    copy_res_end:;
}

