#include <gtest/gtest.h>
#include "image_processor.h"
#include <iterator>

// Test fixture for ImageProcessor class
class ImageProcessorTest : public ::testing::Test {
public:
    static std::string imagePath;
protected:
    ImageProcessor* imgProcessor;

    void SetUp() override {
        // Initialize with the provided image path
        imgProcessor = new ImageProcessor(imagePath);
    }

    void TearDown() override {
        delete imgProcessor;
    }

};

std::string ImageProcessorTest::imagePath;

// Test for flipping an image along the x-axis
TEST_F(ImageProcessorTest, FlipX) {
    EXPECT_NO_THROW(imgProcessor->flip_x());
}

// Test for flipping an image along the y-axis
TEST_F(ImageProcessorTest, FlipY) {
    EXPECT_NO_THROW(imgProcessor->flip_y());
}

// Test for rotating an image to the left
TEST_F(ImageProcessorTest, RotateLeft) {
    EXPECT_NO_THROW(imgProcessor->rotate_left());
}

// Test for rotating an image to the right
TEST_F(ImageProcessorTest, RotateRight) {
    EXPECT_NO_THROW(imgProcessor->rotate_right());
}

// Test for converting an image to grayscale
TEST_F(ImageProcessorTest, Grayscale) {
    EXPECT_NO_THROW(imgProcessor->grayscale());
}

// Test for getting the average pixel density
TEST_F(ImageProcessorTest, GetAveragePixelDensity) {
    double density = imgProcessor->get_average_pixel_density();
    EXPECT_GE(density, 0.0);
}

// Test for writing an image
TEST_F(ImageProcessorTest, WriteImage) {
    EXPECT_NO_THROW(imgProcessor->write("output.jpg", "jpg"));
}

// Test for flipped grayscale function
TEST_F(ImageProcessorTest, FlippedGrayscale) {
    int result = imgProcessor->flipped_grayscale("flipped_output.jpg");
    EXPECT_EQ(result, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_test_image>" << std::endl;
        return 1;
    }
    ImageProcessorTest::imagePath = argv[1];
    return RUN_ALL_TESTS();
}
