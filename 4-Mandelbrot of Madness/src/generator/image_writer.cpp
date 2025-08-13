#include "../headers/generator/image_writer.hpp"
#include <vector>
using namespace std;

Color getColor(int n, int max_iters, const ColorScheme& scheme) {
    if (n == max_iters) return {0, 0, 0};

    float t = static_cast<float>(n) / static_cast<float>(max_iters);

    float r_coeff1 = scheme.pattern_r[0];
    float r_coeff2 = scheme.pattern_r[1];
    float r_coeff3 = scheme.pattern_r[2];
    
    float g_coeff1 = scheme.pattern_g[0];
    float g_coeff2 = scheme.pattern_g[1];
    float g_coeff3 = scheme.pattern_g[2];

    float b_coeff1 = scheme.pattern_b[0];
    float b_coeff2 = scheme.pattern_b[1];
    float b_coeff3 = scheme.pattern_b[2];

    float interp_r = static_cast<unsigned char>(r_coeff1 * powf(1 - t, r_coeff2) * powf(t, r_coeff3) * 255);
    float interp_g = static_cast<unsigned char>(g_coeff1 * powf(1 - t, g_coeff2) * powf(t, g_coeff3) * 255);
    float interp_b = static_cast<unsigned char>(b_coeff1 * powf(1 - t, b_coeff2) * powf(t, b_coeff3) * 255);

    unsigned char r = static_cast<unsigned char>(std::min(255.0f, (scheme.background[0] * 255) + interp_r));
    unsigned char g = static_cast<unsigned char>(std::min(255.0f, (scheme.background[1] * 255) + interp_g));
    unsigned char b = static_cast<unsigned char>(std::min(255.0f, (scheme.background[2] * 255) + interp_b));

    return {r, g, b};
}

#pragma pack(push, 1)   // no padding 
// source: https://gibberlings3.github.io/iesdp/file_formats/ie_formats/bmp.htm

// 14 bytes
struct BmpFileHeader {
    char signature[2]{'B', 'M'};
    uint32_t fileSize{0};
    uint32_t reserved{0};
    uint32_t offsetData{54}; // 14 + 40 = 54 byte
};

// 40 bytes
struct BmpInfoHeader {
    uint32_t size{40};
    int32_t width{0};
    int32_t height{0};
    uint16_t planes{1};
    uint16_t bitCount{3*8}; // 1 byte per color
    uint32_t compression{0};
    uint32_t imageSize{0};
    int32_t xPixelsPM{0}; // pixels per m
    int32_t yPixelsPM{0};
    uint32_t clrUsed{0};
    uint32_t clrImportant{0};
};
#pragma pack(pop)   


void saveToBmp(const string& filename, const Colors& pixels, int width, int height){
    ofstream file(filename, ios::out | ios::binary);
    if (!file) return;

    // mult of 4
    const int paddingAmount = (4 - (width * 3) % 4) % 4;

    BmpFileHeader fileHeader;
    BmpInfoHeader infoHeader;

    infoHeader.width = width;
    infoHeader.height = height;
    
    const int imageSize = (width * 3 + paddingAmount) * height;
    fileHeader.fileSize = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader) + imageSize;

    // write headers
    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    // BMP is written bottoms-up
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            const Color& color = pixels[y * width + x];
            
            unsigned char bgr[] = {color.b, color.g, color.r}; // BGR
            file.write(reinterpret_cast<char*>(bgr), 3);
        }
        
        // add padding
        if (paddingAmount > 0) {
            const char padding[3] = {0, 0, 0};
            file.write(padding, paddingAmount);
        }
    }

    file.close();
}
