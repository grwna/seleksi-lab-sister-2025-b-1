#include "../headers/generator/image_writer.hpp"


Color getColor(int n, int max_iters){
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
