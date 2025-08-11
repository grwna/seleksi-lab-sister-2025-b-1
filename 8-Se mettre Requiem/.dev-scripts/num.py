import sys
sys.set_int_max_str_digits(10001)

# {2013265921, 31},
# {2281701377, 3},
# {469762049, 3}
num = 469762049
k = num.bit_length()
print(k)
barret_mu = ((1 << (2*k)) // num)
print(barret_mu)