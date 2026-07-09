#include <iostream>
#include "Image.h"

int main() {
    Image image("../test.jpg");

    int levels = 4;

    for (int i =  0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth(); j++) {
            Pixel oldPixel = image.getPixel(j, i);
            Pixel newPixel = (oldPixel * (levels-1) / 255) * 255 / (levels-1);
            image.setPixel(j, i, newPixel);
            Pixel error = oldPixel - newPixel;
            if (j+1 < image.getWidth()) {
                image.setPixel(j+1, i, image.getPixel(j+1, i) + ((error * 7/16)));
            }
            if (i+1 < image.getHeight()) {
                if (j-1 >= 0) {
                    image.setPixel(j-1,i+1, image.getPixel(j-1, i+1) + ((error * 3/16)));
                }
                image.setPixel(j, i+1, image.getPixel(j, i+1) + ((error * 5/16)));
                if (j+1 < image.getWidth()) {
                    image.setPixel(j+1,i+1, image.getPixel(j+1, i+1) + ((error * 1/16)));
                }
            }
        }
    }
    std::cout << image.getWidth() << "\n";
    std::cout << image.getHeight() << "\n";
    std::cout << image.getChannels() << "\n";
    image.saveImageJPG("../out.jpg");
    //image.saveImagePNG("../out.png");
    return 0;
}
