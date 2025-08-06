#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <algorithm>
#include <set>

// POSIX headers for process management
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// Type alias for our results map to keep things clean.
// Using std::pair instead of std::array to ensure comparability for map keys.
using ResultsMap = std::map<std::tuple<std::pair<int, int>, int, std::string>, double>;

/**
 * @brief Executes an external program, passes input via stdin, and measures execution time.
 * @param command The command to execute.
 * @param input_str The string to pass to the command's standard input.
 * @param duration Reference to a double where the execution time will be stored.
 * @return True on success, false on failure.
 */
bool run_process(const std::string& command, const std::string& input_str, double& duration) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return false;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return false;
    }

    if (pid == 0) { // --- Child Process ---
        close(pipefd[1]); // Close write end of the pipe
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to read end of the pipe
        close(pipefd[0]);

        // To prevent the child from printing to our console
        int dev_null = open("/dev/null", O_WRONLY);
        if (dev_null != -1) {
            dup2(dev_null, STDOUT_FILENO);
            dup2(dev_null, STDERR_FILENO);
            close(dev_null);
        }

        // Execute the command
        execlp(command.c_str(), command.c_str(), nullptr);
        perror("execlp"); // execlp only returns on error
        _exit(127);
    } else { // --- Parent Process ---
        close(pipefd[0]); // Close read end
        write(pipefd[1], input_str.c_str(), input_str.length());
        close(pipefd[1]); // Close write end to send EOF to child

        int status;
        waitpid(pid, &status, 0);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            std::chrono::duration<double> elapsed = end_time - start_time;
            duration = elapsed.count();
            return true;
        }
        return false;
    }
}

/**
 * @brief Formats and prints benchmark results from a map into a clean table.
 * @param results The map containing the benchmark results.
 */
void print_benchmark_results(const ResultsMap& results) {
    // Get unique (dimension, iteration) pairs
    std::set<std::pair<std::pair<int, int>, int>> unique_combinations;
    for (const auto& pair : results) {
        unique_combinations.insert({std::get<0>(pair.first), std::get<1>(pair.first)});
    }

    for (const auto& combo : unique_combinations) {
        const auto& dim_pair = combo.first;
        const int iter_count = combo.second;

        // Fetch results for each mode
        auto find_time = [&](const std::string& mode) {
            auto it = results.find({dim_pair, iter_count, mode});
            return (it != results.end()) ? it->second : -1.0;
        };

        double serial_time = find_time("SERIAL");
        double cpu_time = find_time("CPU");
        double gpu_time = find_time("GPU");

        if (serial_time < 0) continue; // Skip if no serial data for comparison

        // Calculate speedups
        double cpu_speedup = (cpu_time > 0) ? serial_time / cpu_time : 0.0;
        double gpu_speedup = (gpu_time > 0) ? serial_time / gpu_time : 0.0;

        // Prepare strings for formatting
        std::stringstream ss_serial_t, ss_cpu_t, ss_gpu_t;
        std::stringstream ss_cpu_s, ss_gpu_s;

        ss_serial_t << std::fixed << std::setprecision(6) << serial_time;
        ss_cpu_t << std::fixed << std::setprecision(6) << cpu_time;
        ss_gpu_t << std::fixed << std::setprecision(6) << gpu_time;
        
        ss_cpu_s << std::fixed << std::setprecision(2) << cpu_speedup << "x";
        ss_gpu_s << std::fixed << std::setprecision(2) << gpu_speedup << "x";
        
        std::string serial_t_str = ss_serial_t.str();
        std::string cpu_t_str = (cpu_time >= 0) ? ss_cpu_t.str() : "N/A";
        std::string gpu_t_str = (gpu_time >= 0) ? ss_gpu_t.str() : "N/A";
        std::string serial_s_str = "1.00x";
        std::string cpu_s_str = (cpu_time >= 0) ? ss_cpu_s.str() : "N/A";
        std::string gpu_s_str = (gpu_time >= 0) ? ss_gpu_s.str() : "N/A";

        // Calculate column widths
        size_t time_col_width = std::max({serial_t_str.length(), cpu_t_str.length(), gpu_t_str.length()});
        size_t speedup_col_width = std::max({serial_s_str.length(), cpu_s_str.length(), gpu_s_str.length()});
        size_t mode_col_width = 6; // "Serial"

        // Print table
        std::cout << "\n" << dim_pair.first << " x " << dim_pair.second << ", " << iter_count << " iterations" << std::endl;
        size_t total_width = mode_col_width + time_col_width + speedup_col_width + 7;
        std::cout << std::string(total_width, '-') << std::endl;

        std::cout << std::left << std::setw(mode_col_width) << "Serial" << " | "
                  << std::right << std::setw(time_col_width) << serial_t_str << " | "
                  << std::right << std::setw(speedup_col_width) << serial_s_str << std::endl;
        
        std::cout << std::left << std::setw(mode_col_width) << "CPU" << " | "
                  << std::right << std::setw(time_col_width) << cpu_t_str << " | "
                  << std::right << std::setw(speedup_col_width) << cpu_s_str << std::endl;

        std::cout << std::left << std::setw(mode_col_width) << "GPU" << " | "
                  << std::right << std::setw(time_col_width) << gpu_t_str << " | "
                  << std::right << std::setw(speedup_col_width) << gpu_s_str << std::endl;

        std::cout << std::string(total_width, '-') << std::endl;
    }
}


int main() {
    // --- Configuration ---
    const std::vector<std::pair<int, int>> dimensions = {{2560, 1600}, {5120, 2880}, {7680, 4320}};
    const std::vector<int> iterations = {50, 1000, 10000};
    const std::vector<std::string> modes = {"SERIAL", "CPU", "GPU"};
    const int num_runs = 5;
    const std::string executable = "bin/mandelbrot_cli";

    ResultsMap results;

    // --- Benchmarking Loop ---
    for (int iter : iterations) {
        for (const auto& dim : dimensions) {
            for (const std::string& mode : modes) {
                std::stringstream ss;
                ss << dim.first << "\n" << dim.second << "\n" << iter << "\n" << mode << "\n'none'\n'NOSAVE'";
                std::string inp = ss.str();
                
                std::vector<double> run_times;
                for (int i = 0; i < num_runs; ++i) {
                    double duration;
                    if (run_process(executable, inp, duration)) {
                        run_times.push_back(duration);
                        std::cout << "(" << dim.first << " x " << dim.second << ", " << iter << ", " << mode << ") : " << duration << std::endl;
                    } else {
                        std::cerr << "Failed to run benchmark for " << mode << std::endl;
                        break;
                    }
                }

                if (!run_times.empty()) {
                    double sum = std::accumulate(run_times.begin(), run_times.end(), 0.0);
                    double average = sum / run_times.size();
                    results[{dim, iter, mode}] = average;
                    std::cout << "Average: " << average << "\n" << std::endl;
                }
            }
        }
    }

    // --- Print Final Summary ---
    print_benchmark_results(results);

    return 0;
}
