#pragma once
#include <complex>
#include <vector>
using namespace std;

struct Color {
    unsigned char r, g ,b;
};

struct Bounds {
    double x_min;
    double x_max;
    double y_min;
    double y_max;
};

using Colors = vector<Color>;
using Complex = complex<float>;