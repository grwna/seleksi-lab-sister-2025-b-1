#include "headers/generator/cpu.hpp"
#include "headers/generator/gpu.hpp"
#include "headers/datatypes.hpp"
#include "headers/image_writer.hpp"
#include "stdio.h"
#include <algorithm>

int main(){
    int width;
    int height;
    char mode_buf[10];
    int max_iters;
    char filename_buf[256];
    char save_buf[7];

    scanf("%d", &width);
    scanf("%d", &height);
    scanf("%d", &max_iters);
    scanf("%9s", mode_buf);
    scanf("%255s", filename_buf);
    scanf("%6s", save_buf);
    
    string filename = filename_buf;
    string mode = mode_buf;
    string save = save_buf;
    transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    printf("%d x %d\n%d\n%s\n", width, height, max_iters, filename.c_str());

    Colors result;
    if (mode == "serial") {
        printf("SERIAL\n");
        result = mandelbrotSerial(width, height, max_iters);
    } else if (mode == "cpu") {
        printf("CPU\n");
        result = mandelbrotCPU(width, height, max_iters);
    } else if (mode == "gpu") {
        printf("GPU\n");
        result = mandelbrotGPU(width, height, max_iters);
    }

    printf("%s\n", save.c_str());
    if (save == "SAVE"){
        saveToBmp(filename, result, width, height);
        printf("File saved to %s\n", filename.c_str());
    }
    

    return 0;
}