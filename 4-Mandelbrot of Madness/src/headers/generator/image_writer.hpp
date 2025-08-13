#pragma once
#include "datatypes.hpp"
#include "gui/SharedState.hpp"
#include <fstream>
#include <string>
#include <cstdint>
#include <algorithm>


Color getColor(int n, int max_iter, const ColorScheme& scheme);

void saveToBmp(const std::string& filename, const Colors& pixels, int width, int height);