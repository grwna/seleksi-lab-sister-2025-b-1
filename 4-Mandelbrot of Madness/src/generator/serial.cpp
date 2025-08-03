#include "headers/serial.hpp"
#include "headers/image_writer.hpp"

int calculateMandelbrot(const Complex &c, int max_iters){
    Complex z = 0;
    int n = 0;

    // dont diverge to inf (threshold = 2) 
    while (std::abs(z) <= 2.0 && n < max_iters) {
        z = z * z + c;
        n++;
    }
    return n;
}

Colors mandelbrotSerial(int width, int height, int max_iterations){
    Colors pixels(width * height);

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            // rentang mandelbrot
            double real = -2.0 + (static_cast<double>(x) / (width - 1)) * 3.0;
            double imag = -1.0 + (static_cast<double>(y) / (height - 1)) * 2.0;
            Complex c(real, imag);

            int iterations = calculateMandelbrot(c, max_iterations);
            pixels[y * width + x] = getColor(iterations, max_iterations);  // calculate color for point
        }
    }

return pixels;
}
