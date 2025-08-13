#pragma once
#include "datatypes.hpp"
#include <fstream>
#include <string>
#include <cstdint>
#include <algorithm>
using namespace std;


Color getColor(int n, int max_iter);

void saveToBmp(const string& filename, const Colors& pixels, int width, int height);