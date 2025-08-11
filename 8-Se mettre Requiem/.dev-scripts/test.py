import subprocess
import random
import sys
import time

# previous best 145.96551251411438

sys.set_int_max_str_digits(10000000)
py_results = []
c_results = []

lol = 20 if sys.argv[1] == "A" else 1
for j in range(lol):
    num_digits = int(sys.argv[2]) * (j+1)
    # print(f"Generating {num_digits} digits numbers ")

    start = time.time()
    if sys.argv[1] == "A":
        num1 = random.randint(10**(num_digits-1), 10**num_digits - 1)
        num2 = random.randint(10**(num_digits-1), 10**num_digits - 1)

    if sys.argv[1] == "B":
        num1 = pow(10,int(sys.argv[2]))
        num2 = num1
    print("First 10 of 1: ", (str(num1))[:10])
    print("First 10 of 2: ", (str(num2))[:10])

    print(len(str(num2)))
    print(len(str(num2)))

    print(f"Time generated: {time.time()-start}")
    nums = f"{num1}\n{num2}"
    start = time.time()
    py_result = (num1 * num2)
    print("Python calculation: ", time.time() - start)
    start = time.time()
    result = subprocess.run(
        [sys.argv[3]],
        input=nums,
        shell=True, 
        capture_output=True, 
        text=True,
    )
    print(f"C calculation: {time.time()-start}")

    output = result.stdout.strip()
    c_result = (int(output))
    try:
        assert py_result == c_result
        print("CORRECT\n")
    except:
        print("INCORRECT")
        print("Python: ", py_result)
        print()
        print("C: ", c_result)
        exit()

# Rekor 10.000 digit 87 detik