#include "image_processor.h"
#include <opencv2/opencv.hpp>
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
    read_image(image_path);
}

void ImageProcessor::read_image(const std::string &image_path)
{
    m_raw_image_path = image_path;
    m_raw_image = cv::imread(image_path, cv::IMREAD_COLOR);
    if (m_raw_image.empty())
    {
        throw std::runtime_error("Error: Could not open or find the image.");
    }
}

void ImageProcessor::reset_image()
{
    m_raw_image = cv::imread(m_raw_image_path, cv::IMREAD_COLOR);
    if (m_raw_image.empty())
    {
        throw std::runtime_error("Error: Could not open or find the image.");
    }
}

void ImageProcessor::flip_x()
{
    cv::flip(m_raw_image, m_raw_image, 0); // Flip around x-axis
}

void ImageProcessor::flip_y()
{
    cv::flip(m_raw_image, m_raw_image, 1); // Flip around y-axis
}

void ImageProcessor::rotate_left()
{
    cv::rotate(m_raw_image, m_raw_image, cv::ROTATE_90_COUNTERCLOCKWISE);
}

void ImageProcessor::rotate_right()
{
    cv::rotate(m_raw_image, m_raw_image, cv::ROTATE_90_CLOCKWISE);
}

void ImageProcessor::grayscale() {
    // Check if the image is already grayscale
    if (m_raw_image.channels() == 1) {
        std::cout << "Image is already in grayscale. Skipping conversion." << std::endl;
        return;  // Skip conversion if already grayscale
    }

    // Create a temporary image to hold the grayscale conversion
    cv::Mat grayImage;
    cv::cvtColor(m_raw_image, grayImage, cv::COLOR_BGR2GRAY);

    // Update the image with the grayscale version
    m_raw_image = grayImage;
}


int ImageProcessor::flipped_grayscale(const std::string &output_path)
{
    cv::Mat flipped_img;
    cv::flip(m_raw_image, flipped_img, 1); // Flip around y-axis for example
    cv::Mat gray_img;
    cv::cvtColor(flipped_img, gray_img, cv::COLOR_BGR2GRAY); // Convert flipped image to grayscale
    return cv::imwrite(output_path, gray_img) ? 0 : -1; // Success or failure
}

pybind11::array_t<unsigned char> ImageProcessor::get_image()
{
    cv2eigen(m_raw_image, m_eigen_image);
    return pybind11::array_t<unsigned char>({m_eigen_image.rows(), m_eigen_image.cols()}, m_eigen_image.data()); 
}

void ImageProcessor::write_image(const std::string &output_path, const std::string &format)
{
    std::vector<int> compression_params;
    if (format == "jpg" || format == "jpeg")
    {
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(95); // High quality
    }
    else if (format == "png")
    {
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(3); // Medium compression
    }

    if (!cv::imwrite(output_path, m_raw_image, compression_params))
    {
        throw std::runtime_error("Failed to write image to file.");
    }
}

