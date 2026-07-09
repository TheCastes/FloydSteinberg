#pragma once

#include <iostream>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "external/stb_image.h"
#include "external/stb_image_write.h"

class Pixel {
public:
    int R = 0;
    int G = 0;
    int B = 0;
    int A = 0;

    Pixel operator*(int s) const {
        return Pixel{R * s, G * s, B * s, A * s};
    }
    Pixel operator/(int s) const {
        return Pixel{R / s, G / s, B / s, A / s};
    }

    Pixel operator-(const Pixel & pixel) const {
        return Pixel{R - pixel.R, G - pixel.G, B - pixel.B, A - pixel.A};
    }

    Pixel operator+(int s) const {
        return Pixel{R + s, G + s, B + s, A + s};
    }
    Pixel operator+(const Pixel &p) const {
        return Pixel{R + p.R, G + p.G, B + p.B, A + p.A};
    }
};


class Image {
    public:

    explicit Image(const char *fileName) {
        if ((data = stbi_load(fileName, &width, &height, &channels, 0)) != nullptr) {
            size = width * height * channels;
        } else {
            std::cout << "Error loading image: " << fileName << "\n";
        }
    }

    ~Image() {
        if (data != nullptr) {
            stbi_image_free(data);
        }
        data = nullptr;
        width = 0;
        height = 0;
        channels = 0;
        size = 0;
    }

    int getWidth() const {
        return width;
    }
    int getHeight() const {
        return height;
    }

    int getChannels() const {
        return channels;
    }

    int getRes() const {
        return width * height;
    }

    Pixel getPixel(int x, int y) const {
        Pixel p;
        size_t index = (y * width + x) * channels;
        p.R = data[index];
        p.G = data[index+1];
        p.B = data[index+2];
        if (channels == 4) {
            p.A = data[index+3];
        }
        return p;
    }

    void setPixel(int x, int y, const Pixel &p) const {
        size_t index = (y * width + x) * channels;
        data[index] = std::clamp(p.R, 0, 255);
        data[index+1] = std::clamp(p.G, 0, 255);
        data[index+2] = std::clamp(p.B, 0, 255);
        if (channels == 4) {
            data[index+3] = std::clamp(p.A, 0, 255);
        }
    }

    void saveImageJPG(const char *fileName) const {
        stbi_write_jpg(fileName, width, height, channels, data, 100);
    }

    void saveImagePNG(const char *fileName) const {
        stbi_write_png(fileName, width, height, channels, data, width * channels);
    }

    void toGreyScale() const {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Pixel p = getPixel(x, y);
                int L = static_cast<int>(0.2126 * p.R + 0.7152 * p.G + 0.0722 * p.B);
                setPixel(x, y, Pixel{L, L, L, p.A});
            }
        }
    }

    private:
    int width{};
    int height{};
    int channels{};
    size_t size;
    uint8_t *data;
};

