#include "../headers/generator/gpu.hpp"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdio.h>

using ComplexCU = float2;    // cuda datatype

// __device__ untuk fungsi yang dipanggil di dalam kernel

__device__ Color getColor_device(int n, int max_iters) {
    const unsigned char bg_r = 64;
    const unsigned char bg_g = 0;
    const unsigned char bg_b = 0;
    
    if (n == max_iters) return {0, 0, 0};

    // normalisasi
    float t = static_cast<float>(n)/ static_cast<float> (max_iters);

    // interpolation
    float interp_r = static_cast<unsigned char>(9 * (1 - t) * t * t * t * 255);
    float interp_g = static_cast<unsigned char>(15 * (1 - t) * (1 - t) * t * t * 255);
    float interp_b = static_cast<unsigned char>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

    unsigned char r = static_cast<unsigned char>(min(255.0f, bg_r + interp_r));
    unsigned char g = static_cast<unsigned char>(min(255.0f, bg_g + interp_g));
    unsigned char b = static_cast<unsigned char>(min(255.0f, bg_b + interp_b));

    return {r, g, b};
}

__device__ int calculateMandelbrot_device(const ComplexCU& c, int max_iters) {
    ComplexCU z = {0.0, 0.0};
    int n = 0;
    while (z.x * z.x + z.y * z.y <= 4.0 && n < max_iters) {
        float real_temp = z.x * z.x - z.y * z.y + c.x;
        z.y = 2.0 * z.x * z.y + c.y;
        z.x = real_temp;
        n++;
    }
    return n;
}

// ==============================          KERNEL            ===============================
__global__ void mandelbrotKernel(Color* pixels, int width, int height, int max_iterations, Bounds bounds) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int num_pixels = width * height;

    if (index >= num_pixels) {
        return;
    }

    // Turn idx to pixel coords.
    int x = index % width;
    int y = index / width;

    double x_range = bounds.x_max - bounds.x_min;
    double y_range = bounds.y_max - bounds.y_min;

    float real = bounds.x_min + (static_cast<float>(x) / (width - 1)) * x_range;
    float imag = bounds.y_min + (static_cast<float>(y) / (height - 1)) * y_range;
    ComplexCU c = {real, imag};

    int iterations;
    
    if ((real + 1.0) * (real + 1.0) + imag * imag < 0.0625) { // 1/16
        iterations = max_iterations;
    }
    else {
        float q = (real - 0.25) * (real - 0.25) + imag * imag;
        if (q * (q + (real - 0.25)) < 0.25 * imag * imag) {
            iterations = max_iterations;
        } else {
            iterations = calculateMandelbrot_device(c, max_iterations);
        }
    }
    pixels[index] = getColor_device(iterations, max_iterations);
}

Colors mandelbrotGPU(int width, int height, int max_iterations, const Bounds& bounds) {
    int num_pixels = width * height;
    size_t buffer_size = num_pixels * sizeof(Color);

    Color* pixels = nullptr;
    cudaMalloc(&pixels, buffer_size);

    int threads = 256;
    int blocks = (num_pixels + threads - 1) / threads;
    mandelbrotKernel<<<blocks, threads>>>(pixels, width, height, max_iterations, bounds);
    
    cudaGetLastError(); 

    // Copy device to host (data)
    Colors h_pixels(num_pixels);
    cudaMemcpy(h_pixels.data(), pixels, buffer_size, cudaMemcpyDeviceToHost);
    cudaFree(pixels);

    return h_pixels;
}

__device__ int calculateJulia_device(ComplexCU z, const ComplexCU& c, int max_iters) {
    int n = 0;
    while (z.x * z.x + z.y * z.y <= 4.0f && n < max_iters) {
        float real_temp = z.x * z.x - z.y * z.y + c.x;
        z.y = 2.0f * z.x * z.y + c.y;
        z.x = real_temp;
        n++;
    }
    return n;
}

__global__ void juliaKernel(Color* pixels, int width, int height, int max_iterations, ComplexCU c) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    if (index >= width * height) return;

    int x = index % width;
    int y = index / width;

    // Memetakan koordinat piksel ke bidang kompleks untuk Julia set
    // Rentang [-2, 2] biasanya memberikan hasil yang baik
    float real = -2.0f + (static_cast<float>(x) / (width - 1)) * 4.0f;
    float imag = -2.0f + (static_cast<float>(y) / (height - 1)) * 4.0f;
    ComplexCU z = {real, imag};

    int iterations = calculateJulia_device(z, c, max_iterations);
    pixels[index] = getColor_device(iterations, max_iterations);
}

Colors juliaGPU(int width, int height, int max_iterations, const Complex& c) {
    int num_pixels = width * height;
    size_t buffer_size = num_pixels * sizeof(Color);

    Color* d_pixels = nullptr;
    cudaMalloc(&d_pixels, buffer_size);

    // Konversi konstanta Julia dari tipe host (double) ke tipe device (float)
    ComplexCU c_device = {static_cast<float>(c.real()), static_cast<float>(c.imag())};

    int threads = 256;
    int blocks = (num_pixels + threads - 1) / threads;
    juliaKernel<<<blocks, threads>>>(d_pixels, width, height, max_iterations, c_device);

    cudaGetLastError(); 

    Colors h_pixels(num_pixels);
    cudaMemcpy(h_pixels.data(), d_pixels, buffer_size, cudaMemcpyDeviceToHost);
    cudaFree(d_pixels);

    return h_pixels;
}
