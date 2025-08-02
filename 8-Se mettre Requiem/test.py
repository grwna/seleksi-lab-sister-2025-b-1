import subprocess

py_results = []
c_results = []

for i in range(5):
    nums = open(f"test/{i+1}", "r").readlines()
    py_results.append(str(int(nums[0].strip()) * int(nums[1].strip())))
    result = subprocess.run(
        f"bin/se_mettre < test/{i+1}", 
        shell=True, 
        capture_output=True, 
        text=True,
        cwd="."
    )

    output = result.stdout.strip()
    c_results.append(output)

assert py_results == c_results

with open("test/results_py.txt", "w") as f:
    for result in py_results:
        f.write(str(result) + "\n")
    
with open("test/results_c.txt", "w") as f:
    for result in c_results:
        f.write(str(result) + "\n")


