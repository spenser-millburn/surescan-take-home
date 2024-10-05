#include "image_processor.h"
#include <iostream>

int main() {
    ImageProcessor imgProcessor("input.jpg");
    imgProcessor.flipped_grayscale("./output.jpg");

    std::cout << "Processing complete!" << std::endl;
    return 0;
}
