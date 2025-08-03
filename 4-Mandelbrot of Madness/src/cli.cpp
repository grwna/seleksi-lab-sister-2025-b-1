#include "headers/serial.hpp"
#include "headers/datatypes.hpp"
#include "headers/image_writer.hpp"
#include "stdio.h"
#include <algorithm>

int main(){
    int width;
    int height;
    char filename_buf[256];
    char mode_buf[10];
    int max_iters;

    scanf("%d", &width);
    scanf("%d", &height);
    scanf("%d", &max_iters);
    scanf("%255s", filename_buf);
    scanf("%9s", mode_buf);
    
    string filename = filename_buf;
    string mode = mode_buf;
    transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    printf("%d x %d\n%d\n%s\n", width, height, max_iters, filename.c_str());

    Colors result;
    if (mode == "serial") {
        printf("SERIAL\n");
        result = mandelbrotSerial(width, height, max_iters);
    } else if (mode == "cpu") {
        printf("CPU\n");
    } else if (mode == "gpu") {
        printf("GPU\n");
    }

    saveToBmp(filename, result, width, height);
    

    return 0;
}