#include "../headers/generator/cpu.hpp"

Colors mandelbrotCPU(int width, int height, int max_iters, const Bounds& bounds, const ColorScheme& scheme){
    Colors pixels(width * height);

    double x_range = bounds.x_max - bounds.x_min;
    double y_range = bounds.y_max - bounds.y_min;

    # pragma omp parallel for schedule(dynamic)
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


Colors juliaCPU(int width, int height, int max_iterations, const Complex& c, const ColorScheme& scheme) {
    Colors pixels(width * height);

    # pragma omp parallel for schedule(dynamic)
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
