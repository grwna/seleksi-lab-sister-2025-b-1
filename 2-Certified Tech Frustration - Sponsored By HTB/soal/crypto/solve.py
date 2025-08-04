from sage.all import *

pubkey = Matrix(ZZ, [
    [47, -77, -85],
    [-49, 78, 50],
    [57, -78, 99]
])

c = []

lines = open("enc.txt", "r").readlines
for line in lines():
    line = line.strip()[1:][:-1]
    c.append(vector(eval(line)))

ref = c[0]
diff = []

for vec in c[1:]:
    diff.append(ref - vec)

i_pubkey = pubkey.inverse()
PWE = [x * i_pubkey for x in diff]

p0 = ord('H')
flag = "H" + "".join([chr(p0 - x[0]) for x in PWE])
print(flag)

"""
The encryption process is ci = pi * P + r
Where pi is the i-th 3d vector that contains the plaintext, random int a, and random int b
      P is the public key
      r is a constant vector

Since r is constant, we can eliminate it from the vectors of ciphertext by:
ci - cj, which equals to = (pi * P + r) - (pj * P + r) = (pi - pj) * P
which then gives (pi - pj) = (ci - cj) * P^-1 (or inverse of P), this is the plain_with_errors (PWE) array

in the script above, i used c0 as a reference to eliminate the r in every other vectors
This changes the equation to 
--- (p0 - pj) = (c0 - cj) * P^-1
Which can be represented as
--- (p0 - pj) = PWE
Which can be rearranged as:
--- pj = PWE - p0
Then to get the plaintext (without the randint a and randint b) we write
--- pj[0] = PWE[0] - p0[0]

the format of the flag is "HTB{}", so we know p0 is 'H'
We then get the flag by calculating pj[0] for every index j in the ciphertext


HTB{r3duc1nG_tH3_l4tTicE_l1kE_n0b0dY's_pr0bl3M}



"""