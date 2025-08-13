#include "../headers/generator/serial.hpp"

int cardioidCheck(float real, float imag, int max_iters){
    Complex c(real, imag);
    int iterations;
            
    if ((real + 1.0) * (real + 1.0) + imag * imag < 0.0625) { // 1/16
        iterations = max_iters;
    }
    else {
        float q = (real - 0.25) * (real - 0.25) + imag * imag;
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

Colors mandelbrotSerial(int width, int height, int max_iters, const Bounds& bounds, const ColorScheme& scheme){
    Colors pixels(width * height);

    double x_range = bounds.x_max - bounds.x_min;
    double y_range = bounds.y_max - bounds.y_min;

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            float real = bounds.x_min + (static_cast<float>(x) / (width - 1)) * x_range;
            float imag = bounds.y_min + (static_cast<float>(y) / (height - 1)) * y_range;
            
            int iterations = cardioidCheck(real, imag, max_iters);
            pixels[y * width + x] = getColor(iterations, max_iters, scheme);
        }
    }

    return pixels;  
}

Colors juliaSerial(int width, int height, int max_iterations, const Complex& c, const ColorScheme& scheme) {
    Colors pixels(width * height);
    

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double real = -2.0 + (static_cast<double>(x) / (width - 1)) * 4.0;
            double imag = -2.0 + (static_cast<double>(y) / (height - 1)) * 4.0;
            Complex z(real, imag);

            int n = 0;
            while (std::abs(z) <= 2.0 && n < max_iterations) {
                z = z * z + c;
                n++;
            }

            pixels[y * width + x] = getColor(n, max_iterations, scheme);
        }
    }
    return pixels;
}

