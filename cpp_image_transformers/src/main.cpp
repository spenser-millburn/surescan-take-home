#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <iostream>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

// Function to convert OpenCV Mat to Eigen matrix
void cv2eigen(const cv::Mat& src, Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& dst) {
    dst = Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(src.data, src.rows, src.cols);
}

// Function to convert Eigen matrix back to OpenCV Mat
void eigen2cv(const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& src, cv::Mat& dst) {
    dst = cv::Mat(src.rows(), src.cols(), CV_8UC1, const_cast<unsigned char*>(src.data()));
}

// Flip image along x-axis
pybind11::array_t<unsigned char> flip_x(const std::string &image_path, const std::string &output_path) {
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> eigenImage;
    cv2eigen(image, eigenImage);
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> flippedImage = eigenImage.rowwise().reverse();
    return pybind11::array_t<unsigned char>({flippedImage.rows(), flippedImage.cols()}, flippedImage.data());
}

// Flip image along y-axis
pybind11::array_t<unsigned char> flip_y(const std::string &image_path, const std::string &output_path) {
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> eigenImage;
    cv2eigen(image, eigenImage);
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> flippedImage = eigenImage.colwise().reverse();
    return pybind11::array_t<unsigned char>({flippedImage.rows(), flippedImage.cols()}, flippedImage.data());
}

// Rotate image 90 degrees to the left
pybind11::array_t<unsigned char> rotate_left(const std::string &image_path, const std::string &output_path) {
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> eigenImage;
    cv2eigen(image, eigenImage);
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> rotatedImage = eigenImage.transpose().colwise().reverse();
    return pybind11::array_t<unsigned char>({rotatedImage.rows(), rotatedImage.cols()}, rotatedImage.data());
}

// Rotate image 90 degrees to the right
pybind11::array_t<unsigned char> rotate_right(const std::string &image_path, const std::string &output_path) {
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> eigenImage;
    cv2eigen(image, eigenImage);
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> rotatedImage = eigenImage.transpose().rowwise().reverse();
    return pybind11::array_t<unsigned char>({rotatedImage.rows(), rotatedImage.cols()}, rotatedImage.data());
}

// Convert image to grayscale (assuming input is already grayscale)
pybind11::array_t<unsigned char> grayscale(const std::string &image_path, const std::string &output_path) {
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> eigenImage;
    cv2eigen(image, eigenImage);
    return pybind11::array_t<unsigned char>({eigenImage.rows(), eigenImage.cols()}, eigenImage.data());
}

// Return the Eigen matrix of the image
pybind11::array_t<unsigned char> eigen_matrix(const std::string &image_path, const std::string &output_path) {
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> eigenImage;
    cv2eigen(image, eigenImage);
    return pybind11::array_t<unsigned char>({eigenImage.rows(), eigenImage.cols()}, eigenImage.data());
}

// Function to load, process, and save the image
int flipped_grayscale(const std::string &image_path, const std::string &output_path) {
    // Load the image
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Error: Could not open or find the image." << std::endl;
        return -1;
    }

    // Convert the image to an Eigen matrix
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> eigenImage;
    cv2eigen(image, eigenImage);

    // Transform the image (flip it)
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> flippedImage = eigenImage.colwise().reverse();

    // Convert back to OpenCV Mat
    cv::Mat flippedMat;
    eigen2cv(flippedImage, flippedMat);

    // Save the flipped image
    cv::imwrite(output_path, flippedMat);

    return 0; // Success
}


PYBIND11_MODULE(_core, m) {
    m.def("flipped_grayscale", &flipped_grayscale, "A function that transforms (flips) an image and saves it to the output path.");
    m.def("flip_x", &flip_x, "Flip image along x-axis.");
    m.def("flip_y", &flip_y, "Flip image along y-axis.");
    m.def("rotate_left", &rotate_left, "Rotate image 90 degrees to the left.");
    m.def("rotate_right", &rotate_right, "Rotate image 90 degrees to the right.");
    m.def("grayscale", &grayscale, "Convert image to grayscale.");
    m.def("eigen_matrix", &eigen_matrix, "Return the Eigen matrix of the image.");
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: scikit_build_example

        .. autosummary::
           :toctree: _generate

           flipped_grayscale
           flip_x
           flip_y
           rotate_left
           rotate_right
           grayscale
           eigen_matrix
    )pbdoc";
    m.attr("__version__") = "dev";
}
