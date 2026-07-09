#include <iostream>
#include "Image.h"
#include "Dithering.h"

int main() {
    Image image("../test.jpg");
    const int levels = 4;
    floydSteinbergDither(&image, levels);
    //atkinsonDither(&image);
    std::cout << image.getWidth() << "\n";
    std::cout << image.getHeight() << "\n";
    std::cout << image.getChannels() << "\n";
    image.saveImageJPG("../dith.jpg");
    image.saveImagePNG("../dith.png");
    return 0;
}
