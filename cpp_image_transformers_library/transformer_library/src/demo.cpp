#include "image_processor.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: [" << argv[0] << "] <input_image_filename>" << std::endl;
        return 1;
    }

    ImageProcessor imgProcessor(argv[1]);
    imgProcessor.flipped_grayscale("./output.jpg");

    std::cout << "Processing complete!" << std::endl;
    return 0;
}
