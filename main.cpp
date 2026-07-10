#include <iostream>
#include <string>
#include <cstring>
#include "Image.h"
#include "Dithering.h"

static void printUsage(const char *prog) {
    std::cout << "Usage: " << prog << " <input> <output> [options]\n"
              << "\n"
              << "Apply Floyd-Steinberg or Atkinson algorithm for image dithering.\n"
              << "\n"
              << "Options:\n"
              << "  -a, --algorithm <name>  Dithering algorithm: fs (default) or atkinson\n"
              << "  -l, --levels <n>        Quantization levels per channel, >= 2 (default: 4, Floyd-Steinberg only)\n"
              << "  -h, --help              Show this help message\n"
              << "\n";
}

static bool hasExtension(const std::string &fileName, const char *extension) {
    size_t extensionLength = std::strlen(extension);
    if (fileName.size() < extensionLength) {
        return false;
    }
    std::string tail = fileName.substr(fileName.size() - extensionLength);
    for (char &c : tail) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return tail == extension;
}

int main(int argc, char *argv[]) {
    std::string input;
    std::string output;
    std::string algorithm = "fs";
    int levels = 4;

    for (int i = 1; i < argc; i++) {
        std::string argument = argv[i];
        if (argument == "-h" || argument == "--help") {
            printUsage(argv[0]);
            return 0;
        }
        if (argument == "-a" || argument == "--algorithm") {
            if (++i >= argc) {
                std::cerr << "Error: " << argument << " requires a value\n";
                return 1;
            }
            algorithm = argv[i];
        } else if (argument == "-l" || argument == "--levels") {
            if (++i >= argc) {
                std::cerr << "Error: " << argument << " requires a value\n";
                return 1;
            }
            try {
                levels = std::stoi(argv[i]);
            } catch (...) {
                std::cerr << "Error: invalid levels value: " << argv[i] << "\n";
                return 1;
            }
        } else if (!argument.empty() && argument[0] == '-') {
            std::cerr << "Error: unknown option: " << argument << "\n";
            printUsage(argv[0]);
            return 1;
        } else if (input.empty()) {
            input = argument;
        } else if (output.empty()) {
            output = argument;
        } else {
            std::cerr << "Error: unexpected argument: " << argument << "\n";
            return 1;
        }
    }

    if (input.empty() || output.empty()) {
        printUsage(argv[0]);
        return 1;
    }
    if (algorithm != "fs" && algorithm != "atkinson") {
        std::cerr << "Error: unknown algorithm: " << algorithm << " (expected fs (Floyd-Steinberg) or atkinson)\n";
        return 1;
    }
    if (levels < 2) {
        std::cerr << "Error: levels must be >= 2\n";
        return 1;
    }

    bool isPNG = hasExtension(output, ".png");
    bool isJPG = hasExtension(output, ".jpg") || hasExtension(output, ".jpeg");
    if (!isPNG && !isJPG) {
        std::cerr << "Error: output extension must be .png, .jpg or .jpeg\n";
        return 1;
    }

    Image image(input.c_str());
    if (!image.isValid()) {
        return 1;
    }

    if (algorithm == "fs") {
        floydSteinbergDither(&image, levels);
    } else {
        atkinsonDither(&image);
    }

    if (isPNG) {
        image.saveImagePNG(output.c_str());
    } else {
        image.saveImageJPG(output.c_str());
    }

    std::cout << "Saved " << output << " (" << image.getWidth() << "x" << image.getHeight()
              << ", " << image.getChannels() << " channels)\n";
    return 0;
}