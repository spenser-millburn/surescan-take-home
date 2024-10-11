#include "image_processor.h"
#include <stdexcept>

void Processor::cv2eigen(const cv::Mat& src, Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& dst) 
{
    dst = Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(src.data, src.rows, src.cols);
}

void Processor::eigen2cv(const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& src, cv::Mat& dst) 
{
    dst = cv::Mat(src.rows(), src.cols(), CV_8UC1, const_cast<unsigned char*>(src.data()));
}

ImageProcessor::ImageProcessor(const std::string &image_path) 
{
    raw_image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    if (raw_image.empty()) 
    {
        throw std::runtime_error("Error: Could not open or find the image.");
    }
    cv2eigen(raw_image, eigenImage);
}

pybind11::array_t<unsigned char> ImageProcessor::flip_x() 
{
    eigenImage = eigenImage.rowwise().reverse();
    return get_image();
}

pybind11::array_t<unsigned char> ImageProcessor::flip_y() 
{
    eigenImage = eigenImage.colwise().reverse();
    return get_image();
}

pybind11::array_t<unsigned char> ImageProcessor::rotate_left() 
{
    eigenImage = eigenImage.transpose().colwise().reverse();
    return get_image();
}

pybind11::array_t<unsigned char> ImageProcessor::rotate_right() 
{
    eigenImage = eigenImage.transpose().rowwise().reverse();
    return get_image();
}

pybind11::array_t<unsigned char> ImageProcessor::grayscale() 
{
    return get_image();
}

int ImageProcessor::flipped_grayscale(const std::string &output_path) 
{
    eigenImage = eigenImage.colwise().reverse();
    cv::Mat flippedMat;
    eigen2cv(eigenImage, flippedMat);
    cv::imwrite(output_path, flippedMat);
    return 0; // Success
}

pybind11::array_t<unsigned char> ImageProcessor::get_image()
{
    return pybind11::array_t<unsigned char>({eigenImage.rows(), eigenImage.cols()}, eigenImage.data());
}

void ImageProcessor::write_image(const std::string &output_path, const std::string &format) {
    cv::Mat outputMat;
    eigen2cv(eigenImage, outputMat);
    std::vector<int> compression_params;
    if (format == "jpg" || format == "jpeg") {
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(95); // Default quality
    } else if (format == "png") {
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(3); // Default compression level
    }
    cv::imwrite(output_path, outputMat, compression_params);
}
