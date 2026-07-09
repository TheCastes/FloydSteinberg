#pragma once
#include "Image.h"

inline void floydSteinbergDither(Image *image, int levels) {
    for (int i =  0; i < image->getHeight(); i++) {
        for (int j = 0; j < image->getWidth(); j++) {
            Pixel oldPixel = image->getPixel(j, i);
            Pixel newPixel = (oldPixel * (levels-1) / 255) * 255 / (levels-1);
            image->setPixel(j, i, newPixel);
            Pixel error = oldPixel - newPixel;
            if (j+1 < image->getWidth()) {
                image->setPixel(j+1, i, image->getPixel(j+1, i) + ((error * 7/16)));
            }
            if (i+1 < image->getHeight()) {
                if (j-1 >= 0) {
                    image->setPixel(j-1,i+1, image->getPixel(j-1, i+1) + ((error * 3/16)));
                }
                image->setPixel(j, i+1, image->getPixel(j, i+1) + ((error * 5/16)));
                if (j+1 < image->getWidth()) {
                    image->setPixel(j+1,i+1, image->getPixel(j+1, i+1) + ((error * 1/16)));
                }
            }
        }
    }
}

inline void atkinsonDither(Image *image) {
    image->toGreyScale();

    for (int i =  0; i < image->getHeight(); i++) {
        for (int j = 0; j < image->getWidth(); j++) {
            Pixel oldPixel = image->getPixel(j, i);
            Pixel newPixel;
            if (oldPixel.R < 128) {
                newPixel.R = 0;
                newPixel.G = 0;
                newPixel.B = 0;
            } else {
                newPixel.R = 255;
                newPixel.G = 255;
                newPixel.B = 255;
            }
            image->setPixel(j, i, newPixel);
            Pixel error = oldPixel - newPixel;
            if (j+1 < image->getWidth()) {
                image->setPixel(j+1, i, image->getPixel(j+1, i) + ((error * 1/8)));
            }
            if (j+2 < image->getWidth()) {
                image->setPixel(j+2, i, image->getPixel(j+2, i) + ((error * 1/8)));
            }
            if (i+1 < image->getHeight()) {
                if (j-1 >= 0) {
                    image->setPixel(j-1,i+1, image->getPixel(j-1, i+1) + ((error * 1/8)));
                }
                image->setPixel(j, i+1, image->getPixel(j, i+1) + ((error * 1/8)));
                if (j+1 < image->getWidth()) {
                    image->setPixel(j+1,i+1, image->getPixel(j+1, i+1) + ((error * 1/8)));
                }
            }
            if (i+2 < image->getHeight()) {
                image->setPixel(j, i+2, image->getPixel(j, i+2) + ((error * 1/8)));
            }

        }
    }

}
