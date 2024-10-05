#include <iostream>
#include "image_transformers.h" // Include the header for your library

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_image> <output_image>" << std::endl;
        return -1;
    }

    std::string inputImagePath = argv[1];
    std::string outputImagePath = argv[2];

    // Create an ImageProcessor object
    ImageProcessor processor(inputImagePath);

    // Rotate the image to the right
    auto rotatedImageArray = processor.rotate_right();



}
