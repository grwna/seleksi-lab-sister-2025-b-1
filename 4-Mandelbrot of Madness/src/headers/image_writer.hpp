#pragma once
#include "datatypes.hpp"
#include <fstream>
#include <string>
#include <cstdint>

Color getColor(int n, int max_iter);

void saveToBmp(const string& filename, const Colors& pixels, int width, int height);