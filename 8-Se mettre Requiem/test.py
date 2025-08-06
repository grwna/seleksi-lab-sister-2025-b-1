import subprocess

py_results = []
c_results = []

for i in range(6):
    nums = open(f"test/{i+1}", "r").readlines()
    py_results.append(str(int(nums[0].strip()) * int(nums[1].strip())))
    result = subprocess.run(
        f"bin/se < test/{i+1}", 
        shell=True, 
        capture_output=True, 
        text=True,
        cwd="."
    )

    output = result.stdout.strip()
    c_results.append(output)

try:
    assert py_results == c_results
except:
    for i in range(len(py_results)):
        if py_results[i] != c_results[i]:
            print(py_results[i])
            print()
            print(c_results[i])

with open("test/results_py.txt", "w") as f:
    for result in py_results:
        f.write(str(result) + "\n")
    
with open("test/results_c.txt", "w") as f:
    for result in c_results:
        f.write(str(result) + "\n")


