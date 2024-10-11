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
    m_raw_image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    if (m_raw_image.empty()) 
    {
        throw std::runtime_error("Error: Could not open or find the image.");
    }
    cv2eigen(m_raw_image, m_eigen_image);
}

void ImageProcessor::flip_x() 
{
    m_eigen_image = m_eigen_image.rowwise().reverse();
}

void ImageProcessor::flip_y() 
{
    m_eigen_image = m_eigen_image.colwise().reverse();
}

void ImageProcessor::rotate_left() 
{
    m_eigen_image = m_eigen_image.transpose().colwise().reverse();
}

void ImageProcessor::rotate_right() 
{
    m_eigen_image = m_eigen_image.transpose().rowwise().reverse();
}

void ImageProcessor::grayscale() 
{
}

int ImageProcessor::flipped_grayscale(const std::string &output_path) 
{
    m_eigen_image = m_eigen_image.colwise().reverse();
    cv::Mat l_flipped_mat;
    eigen2cv(m_eigen_image, l_flipped_mat);
    cv::imwrite(output_path, l_flipped_mat);
    return 0; // Success
}

pybind11::array_t<unsigned char> ImageProcessor::get_image()
{
    return pybind11::array_t<unsigned char>({m_eigen_image.rows(), m_eigen_image.cols()}, m_eigen_image.data());
}

void ImageProcessor::write_image(const std::string &output_path, const std::string &format) {
    cv::Mat outputMat;
    eigen2cv(m_eigen_image, outputMat);
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
