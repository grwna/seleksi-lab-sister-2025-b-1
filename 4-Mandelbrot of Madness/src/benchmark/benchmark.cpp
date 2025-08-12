#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 

using namespace std;

void run_process(const string& command, const string& input_str, double& duration) {
    int pipefd[2];
    pipe(pipefd);

    auto start_time = chrono::high_resolution_clock::now();
    pid_t pid = fork();

    if (pid == 0) { 
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        int dev_null = open("/dev/null", O_WRONLY);
        if (dev_null != -1) {
            dup2(dev_null, STDOUT_FILENO);
            dup2(dev_null, STDERR_FILENO);
            close(dev_null);
        }

        execlp(command.c_str(), command.c_str(), nullptr);
        _exit(1);
    } else {
        close(pipefd[0]);
        write(pipefd[1], input_str.c_str(), input_str.length());
        close(pipefd[1]);

        waitpid(pid, nullptr, 0); 
        auto end_time = chrono::high_resolution_clock::now();
        
        chrono::duration<double> elapsed = end_time - start_time;
        duration = elapsed.count();
    }
}


int main() {
    const vector<pair<int, int>> dimensions = {{2560, 1600}, {5120, 2880}, {7680, 4320}};
    const vector<int> iterations = {50, 1000, 10000};
    const vector<string> modes = {"SERIAL", "CPU", "GPU"};
    const string executable = "bin/mandelbrot_cli";

    cout << "--- Starting Benchmark ---\n" << endl;

    for (int iter : iterations) {
        for (const auto& dim : dimensions) {
            double serial_duration = 0.0;
            for (const string& mode : modes) {
                stringstream ss;
                ss << dim.first << "\n" << dim.second << "\n" << iter << "\n" << mode << "\n'none'\n'NOSAVE'";
                string inp = ss.str();
                
                double duration = 0.0;
                
                cout << fixed << setprecision(6)
                << "(" << dim.first << "x" << dim.second 
                << ", " << iter << " iter, " << mode 
                << ") -> " << flush;

                run_process(executable, inp, duration);
                cout << fixed << setprecision(6) << "Time: " << duration << "s"; 
                if (mode == "SERIAL") {
                    serial_duration = duration;
                    cout << " (1.00x)" << endl;
                } else {
                    double speedup = (serial_duration > 0 && duration > 0) ? serial_duration / duration : 0.0;
                    cout <<  " (" << fixed << setprecision(2) << speedup << "x)" << endl;
                }
            }
            cout << endl;
        }
    }

    cout << "\n--- Benchmark Finished ---\n" << endl;

    return 0;
}
