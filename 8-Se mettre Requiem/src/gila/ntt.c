#include "ntt.h"

#define MAX_N 2097152 // 2^21, cukup untuk 10^1mil

static ll ntt_a[MAX_N];
static ll ntt_b[MAX_N];
static ll ntt_c[MAX_N];

ll power(ll a, ll b, ll modulo) {
    ll res = 1;
    a = mod(a, modulo);
    loop:
        if (LESSER_OR_EQUAL( b, 0)) goto end;
        if (EQUAL(mod(b,2), 1)) res = mod((mult(res, a)),  modulo);
        a = mod(mult(a,a),  modulo);
        b = div(b, 2);
        goto loop;
    end:
    return res;
}

// modinv
ll inverse(ll n, ll modulo) {
    return power(n, sub(modulo, 2), modulo);
}

// iterative in place ntt
void ntt(ll* arr, int n, ll prime, ll root, int is_inverse) {
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
            ll temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
        i = add(i, 1);
        goto bit_reversal_loop;
    bit_reversal_end:;

    // Cooley-Tukey
    int len = 2;
    main_loop:
        if (GREATER(len, n)) goto main_loop_end;
        ll w_len = power(root, div((sub(prime, 1)), len), prime);
        if (is_inverse) {
            w_len = inverse(w_len, prime);
        }
        i = 0;
        outer_butterfly_loop:
            if (GREATER_OR_EQUAL(i, n)) goto outer_butterfly_end;
            ll w = 1;
            j = 0;
            inner_butterfly_loop:
                if (GREATER_OR_EQUAL(j, div(len, 2))) goto inner_butterfly_end;
                ll u = arr[add(i,j)];
                ll v = mod(mult(arr[add(add(i,j), div(len, 2))], w), prime);
                arr[add(i,j)] = mod(add(u, v), prime);
                arr[add(add(i,j), div(len, 2))] = mod((add(sub(u, v), prime)), prime);
                w = mod((mult(w, w_len)), prime);
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
        ll n_inv = inverse(n, prime);
        i = 0;
        scale_loop:
            if (GREATER_OR_EQUAL(i, n)) goto scale_end;
        arr[i] = mod(mult(arr[i], n_inv), prime);
            i = add(i, 1);
            goto scale_loop;
        scale_end:;
    }
}

// main ntt function
void multiply_ntt(ll res[], int num1[], int len1, int num2[], int len2) {
    // determine transformation size
    int n = 1;
    size_loop:
        if (n >= add(len1, len2)) goto size_end;
        n <<= 1;
        goto size_loop;
    size_end:;

    // kolom 0 - prime (p), kolom 1 - root (r), kolom 2 - mu constant
    unsigned __int128 PRIMES[][3] = {
        {2013265921, 31, 2290649223ULL},
        {2281701377, 3, 8084644318ULL},
        {469762049, 3, 613566755ULL}
    };

    ll prime = sub(0,1), root = sub(0, 1);
    int i = 0;
    prime_loop:
        if (GREATER_OR_EQUAL(i ,3)) goto prime_end; // 3 adalah jumlah prime di daftar
        if (EQUAL(mod((sub(PRIMES[i][0], 1)), n), 0)) {
            prime = PRIMES[i][0];
            root = PRIMES[i][1];
            goto prime_end;
        }
        i = add(i, 1);
        goto prime_loop;
    prime_end:;

    // copy digit (ntt_a, ntt_b)
    i = 0;
    copy_a_loop:
        if (GREATER_OR_EQUAL(i, len1)) goto copy_a_end;
        ntt_a[i] = num1[i];
        i = add(i, 1);
        goto copy_a_loop;
    copy_a_end:;
    zero_pad_a_loop: // pad with zero
        if (GREATER_OR_EQUAL(i, n)) goto zero_pad_a_end;
        ntt_a[i] = 0;
        i = add(i, 1);
        goto zero_pad_a_loop;
    zero_pad_a_end:;

    i = 0;
    copy_b_loop:
        if (GREATER_OR_EQUAL(i, len2)) goto copy_b_end;
        ntt_b[i] = num2[i];
        i = add(i, 1);
        goto copy_b_loop;
    copy_b_end:;
    zero_pad_b_loop:
        if (GREATER(i, n)) goto zero_pad_b_end;
        ntt_b[i] = 0;
        i = add(i, 1);
        goto zero_pad_b_loop;
    zero_pad_b_end:;

    ntt(ntt_a, n, prime, root, 0);
    ntt(ntt_b, n, prime, root, 0);

    // pointwise multiplication in frequency domain
    i = 0;
    pointwise_mult_loop:
        if (GREATER_OR_EQUAL(i, n)) goto pointwise_mult_end;
        ntt_c[i] = mod((mult(ntt_a[i], ntt_b[i])), prime);
        i = add(i, 1);
        goto pointwise_mult_loop;
    pointwise_mult_end:;

    // invers NTT to return to time domain
    ntt(ntt_c, n, prime, root, 1);

    i = 0;
    copy_res_loop:
        if (GREATER_OR_EQUAL(i, n)) goto copy_res_end;
        res[i] = ntt_c[i];
        i = add(i, 1);
        goto copy_res_loop;
    copy_res_end:;
}

