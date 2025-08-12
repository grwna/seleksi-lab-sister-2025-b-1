import subprocess
import time


def print_benchmark_results(results):
    """
    2560 x 1980, 1000 iterations
    ----------------------------
    Serial | 1.02345    |  0%
    CPU    | 0.000234   |  25%
    GPU    | 0.00000112 |  76%
    ----------------------------
    """
    unique_combinations = sorted(list(set((k[0], k[1]) for k in results.keys())))
    for dim, iter_count in unique_combinations:
        serial_time = results.get((dim, iter_count, "SERIAL"))
        cpu_time = results.get((dim, iter_count, "CPU"))
        gpu_time = results.get((dim, iter_count, "GPU"))

        cpu_speedup = serial_time / cpu_time if cpu_time and cpu_time > 0 else float('inf')
        gpu_speedup = serial_time / gpu_time if gpu_time and gpu_time > 0 else float('inf')

        data_rows = {
            "Serial": (f"{serial_time:.6f}", "1.00x"),
            "CPU": (f"{cpu_time:.6f}" if cpu_time is not None else "N/A", f"{cpu_speedup:.2f}x"),
            "GPU": (f"{gpu_time:.6f}" if gpu_time is not None else "N/A", f"{gpu_speedup:.2f}x"),
        }

        time_col_width = max(len(row[0]) for row in data_rows.values())
        speedup_col_width = max(len(row[1]) for row in data_rows.values())
        mode_col_width = len("Serial") # The longest mode name

        header = f"{dim[0]} x {dim[1]}, {iter_count} iterations"
        print(f"\n{header}")
        
        total_width = mode_col_width + time_col_width + speedup_col_width + len(" |  | ")
        print('-' * total_width)

        for mode, (time_str, speedup_str) in data_rows.items():
            print(f"{mode:<{mode_col_width}} | {time_str:>{time_col_width}} | {speedup_str:>{speedup_col_width}}")
        print('-' * total_width)

dimensions = [
    [2560, 1600],
    [5120, 2880],
    [7680, 4320],
]

iterations = [
    50,
    1000,
    10000,
]

modes = [
    "SERIAL",
    "CPU",
    "GPU",
]

results = {}
rep_per_combo = 1
for iter in iterations:
    for dim in dimensions: 
        for mode in modes:
            inp = f"{dim[0]}\n{dim[1]}\n{iter}\n{mode}\n'none'\n'NOSAVE'"
            curr_res = 0

            for i in range(rep_per_combo):
                start = time.perf_counter()
                subprocess.run(
                    ["bin/mandelbrot_cli"], 
                    input=inp, 
                    text=True, 
                    check=True,
                    capture_output=True
                )
                end = time.perf_counter() - start
                curr_res += end
                if mode == "SERIAL":
                    serial = end
                print(f"({dim[0]} x {dim[1]}, {iter}, {mode}) -> Time :  {end} ({serial/end}x)")
            if rep_per_combo > 1:
                curr_res /= rep_per_combo
                print(f"Average: {curr_res}\n")
            results[(tuple(dim), iter, mode)] = curr_res
        print()
print(print_benchmark_results(results))