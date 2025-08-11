import math

def is_prime(n):
    """Checks if a number is prime."""
    if n < 2:
        return False
    if n == 2 or n == 3:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

def prime_factors(n):
    """Finds all unique prime factors of a number."""
    factors = set()
    d = 2
    temp = n
    while d * d <= temp:
        if temp % d == 0:
            factors.add(d)
            while temp % d == 0:
                temp //= d
        d += 1
    if temp > 1:
        factors.add(temp)
    return factors

def verify_ntt_parameters(p, g, mu):
    """
    Verifies if the given parameters (p, g, mu) are valid for an NTT.
    p: prime modulus
    g: primitive root
    mu: Barrett constant
    """
    print(f"--- Verifying parameters: p={p}, g={g}, mu={mu} ---")

    # 1. Primality check
    if not is_prime(p):
        print(f"❌ Failed: The modulus {p} is not a prime number.")
        return False
    print(f"✅ Passed: Modulus {p} is a prime number.")

    # 2. Primitive root check
    p_minus_1 = p - 1
    factors_of_p_minus_1 = prime_factors(p_minus_1)
    
    is_g_primitive = True
    for q in factors_of_p_minus_1:
        if pow(g, p_minus_1 // q, p) == 1:
            print(f"❌ Failed: {g} is not a primitive root. Order is a divisor of {p_minus_1 // q}.")
            is_g_primitive = False
            break
    
    if is_g_primitive:
        print(f"✅ Passed: {g} is a primitive root of {p}.")
    else:
        return False
    
    # 3. Barrett constant check
    k = p.bit_length()
    mu_calculated = (1 << (2 * k)) // p
    
    if mu_calculated == mu:
        print(f"✅ Passed: The Barrett constant {mu} is correct.")
    else:
        print(f"❌ Failed: Incorrect Barrett constant. Expected {mu_calculated}, but got {mu}.")
        return False

    print("--- All parameters are valid! ---\n")
    return True

parameters_to_verify = [
    (2013265921, 31, 2290649223),
    (2281701377, 3, 8084644318),
    (469762049, 3, 613566755),
    (12345, 2, 0) # Example of an invalid set
]

for p, g, mu in parameters_to_verify:
    verify_ntt_parameters(p, g, mu)

"""
    unsigned __int128 PRIMES[][3] = {
        {2013265921, 31, 2290649223ULL},
        {2281701377, 3, 8084644318ULL},
        {469762049, 3, 613566755ULL}
    };
"""