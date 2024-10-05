#include "image_processor.h"
#include <stdexcept>

void Processor::cv2eigen(const cv::Mat& src, Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& dst) {
    dst = Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(src.data, src.rows, src.cols);
}

void Processor::eigen2cv(const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& src, cv::Mat& dst) {
    dst = cv::Mat(src.rows(), src.cols(), CV_8UC1, const_cast<unsigned char*>(src.data()));
}

ImageProcessor::ImageProcessor(const std::string &image_path) {
    image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        throw std::runtime_error("Error: Could not open or find the image.");
    }
    cv2eigen(image, eigenImage);
}

pybind11::array_t<unsigned char> ImageProcessor::flip_x() {
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> flippedImage = eigenImage.rowwise().reverse();
    return pybind11::array_t<unsigned char>({flippedImage.rows(), flippedImage.cols()}, flippedImage.data());
}

pybind11::array_t<unsigned char> ImageProcessor::flip_y() {
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> flippedImage = eigenImage.colwise().reverse();
    return pybind11::array_t<unsigned char>({flippedImage.rows(), flippedImage.cols()}, flippedImage.data());
}

pybind11::array_t<unsigned char> ImageProcessor::rotate_left() {
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> rotatedImage = eigenImage.transpose().colwise().reverse();
    return pybind11::array_t<unsigned char>({rotatedImage.rows(), rotatedImage.cols()}, rotatedImage.data());
}

pybind11::array_t<unsigned char> ImageProcessor::rotate_right() {
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> rotatedImage = eigenImage.transpose().rowwise().reverse();
    return pybind11::array_t<unsigned char>({rotatedImage.rows(), rotatedImage.cols()}, rotatedImage.data());
}

pybind11::array_t<unsigned char> ImageProcessor::grayscale() {
    return pybind11::array_t<unsigned char>({eigenImage.rows(), eigenImage.cols()}, eigenImage.data());
}

pybind11::array_t<unsigned char> ImageProcessor::eigen_matrix() {
    return pybind11::array_t<unsigned char>({eigenImage.rows(), eigenImage.cols()}, eigenImage.data());
}

int ImageProcessor::flipped_grayscale(const std::string &output_path) {
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> flippedImage = eigenImage.colwise().reverse();
    cv::Mat flippedMat;
    eigen2cv(flippedImage, flippedMat);
    cv::imwrite(output_path, flippedMat);
    return 0; // Success
}
