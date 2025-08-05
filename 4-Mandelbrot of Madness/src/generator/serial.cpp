#include "../headers/generator/serial.hpp"

int cardioidCheck(double real, double imag, int max_iters){
    Complex c(real, imag);
    int iterations;
            
    if ((real + 1.0) * (real + 1.0) + imag * imag < 0.0625) { // 1/16
        iterations = max_iters;
    }
    else {
        double q = (real - 0.25) * (real - 0.25) + imag * imag;
        if (q * (q + (real - 0.25)) < 0.25 * imag * imag) {
            iterations = max_iters;
        } else {
            iterations = calculateMandelbrot(c, max_iters);
        }
    }
    return iterations;
}

int calculateMandelbrot(const Complex &c, int max_iters){
    Complex z = 0;
    int n = 0;

    // dont diverge to inf (threshold = 2) 
    while (abs(z) <= 2.0 && n < max_iters) {
        z = z * z + c;
        n++;
    }
    return n;
}

Colors mandelbrotSerial(int width, int height, int max_iters){
    Colors pixels(width * height);

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            double real = -2.0 + (static_cast<double>(x) / (width - 1)) * 3.0;
            double imag = -1.0 + (static_cast<double>(y) / (height - 1)) * 2.0;
            
            int iterations = cardioidCheck(real, imag, max_iters);
            pixels[y * width + x] = getColor(iterations, max_iters);
        }
    }

    return pixels;  
}
