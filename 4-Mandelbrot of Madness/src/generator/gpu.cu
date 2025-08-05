#include "../headers/generator/gpu.hpp"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdio.h>

using ComplexCU = double2;    // cuda datatype

// __device__ untuk fungsi yang dipanggil di dalam kernel

__device__ Color getColor_device(int n, int max_iters) {
    if (n == max_iters) return {0, 0, 0};

    // normalisasi
    double t = static_cast<double>(n)/ static_cast<double> (max_iters);

    // interpolation
    unsigned char r = static_cast<unsigned char>(9 * (1 - t) * t * t * t * 255);
    unsigned char g = static_cast<unsigned char>(15 * (1 - t) * (1 - t) * t * t * 255);
    unsigned char b = static_cast<unsigned char>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

    return {r, g, b};
}

__device__ int calculateMandelbrot_device(const ComplexCU& c, int max_iters) {
    ComplexCU z = {0.0, 0.0};
    int n = 0;
    while (z.x * z.x + z.y * z.y <= 4.0 && n < max_iters) {
        double real_temp = z.x * z.x - z.y * z.y + c.x;
        z.y = 2.0 * z.x * z.y + c.y;
        z.x = real_temp;
        n++;
    }
    return n;
}

// ==============================          KERNEL            ===============================
__global__ void mandelbrotKernel(Color* pixels, int width, int height, int max_iterations) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int num_pixels = width * height;

    if (index >= num_pixels) {
        return;
    }

    // Turn idx to pixel coords.
    int x = index % width;
    int y = index / width;

    double real = -2.0 + (static_cast<double>(x) / (width - 1)) * 3.0;
    double imag = -1.0 + (static_cast<double>(y) / (height - 1)) * 2.0;
    ComplexCU c = {real, imag};

    int iterations;
    
    if ((real + 1.0) * (real + 1.0) + imag * imag < 0.0625) { // 1/16
        iterations = max_iterations;
    }
    else {
        double q = (real - 0.25) * (real - 0.25) + imag * imag;
        if (q * (q + (real - 0.25)) < 0.25 * imag * imag) {
            iterations = max_iterations;
        } else {
            iterations = calculateMandelbrot_device(c, max_iterations);
        }
    }
    pixels[index] = getColor_device(iterations, max_iterations);
}

Colors mandelbrotGPU(int width, int height, int max_iterations) {
    int num_pixels = width * height;
    size_t buffer_size = num_pixels * sizeof(Color);

    Color* pixels = nullptr;
    cudaMalloc(&pixels, buffer_size);

    int threads = 256;
    int blocks = (num_pixels + threads - 1) / threads;
    mandelbrotKernel<<<blocks, threads>>>(pixels, width, height, max_iterations);
    
    cudaGetLastError(); 

    // Copy device to host (data)
    Colors h_pixels(num_pixels);
    cudaMemcpy(h_pixels.data(), pixels, buffer_size, cudaMemcpyDeviceToHost);
    cudaFree(pixels);

    return h_pixels;
}