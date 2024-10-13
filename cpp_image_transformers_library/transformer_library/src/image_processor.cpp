#include "image_processor.h"
#include <opencv2/opencv.hpp>
#include <stdexcept>

void ImageProcessor::cv2eigen(const cv::Mat& src, Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& dst)
{
    dst = Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(src.data, src.rows, src.cols);
}

void ImageProcessor::eigen2cv(const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& src, cv::Mat& dst)
{
    dst = cv::Mat(src.rows(), src.cols(), CV_8UC1, const_cast<unsigned char*>(src.data()));
}

ImageProcessor::ImageProcessor(const std::string &image_path)
{
    read(image_path);
}

void ImageProcessor::read(const std::string &image_path)
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

double ImageProcessor::get_average_pixel_density()
{
    if (m_raw_image.empty())
    {
        std::cerr << "Image is empty." << std::endl;
        return 0.0;
    }

    int l_numPixels = m_raw_image.rows * m_raw_image.cols;
    double l_sum = 0.0;

    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> l_eigen_image;
    if (m_raw_image.channels() == 1)
    {
        cv2eigen(m_raw_image, l_eigen_image);
        for (int i = 0; i < l_eigen_image.rows(); ++i)
        {
            for (int j = 0; j < l_eigen_image.cols(); ++j)
            {
                l_sum += l_eigen_image(i, j);
            }
        }
    }
    else if (m_raw_image.channels() == 3)
    {
        cv::Mat l_channels[3];
        cv::split(m_raw_image, l_channels);

        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> l_eigen_b, l_eigen_g, l_eigen_r;

        cv2eigen(l_channels[0], l_eigen_b);
        cv2eigen(l_channels[1], l_eigen_g);
        cv2eigen(l_channels[2], l_eigen_r);

        for (int i = 0; i < l_eigen_b.rows(); ++i)
        {
            for (int j = 0; j < l_eigen_b.cols(); ++j)
            {
                l_sum += (l_eigen_b(i, j) + l_eigen_g(i, j) + l_eigen_r(i, j)) / 3.0;
            }
        }
    }

    return l_sum / l_numPixels;
}

void ImageProcessor::flip_x()
{
    cv::flip(m_raw_image, m_raw_image, 0);
}

void ImageProcessor::flip_y()
{
    cv::flip(m_raw_image, m_raw_image, 1);
}

void ImageProcessor::rotate_left()
{
    cv::rotate(m_raw_image, m_raw_image, cv::ROTATE_90_COUNTERCLOCKWISE);
}

void ImageProcessor::rotate_right()
{
    cv::rotate(m_raw_image, m_raw_image, cv::ROTATE_90_CLOCKWISE);
}

void ImageProcessor::grayscale()
{
    if (m_raw_image.channels() == 1)
    {
        std::cout << "Image is already in grayscale. Skipping conversion." << std::endl;
        return;
    }

    cv::Mat grayImage;
    cv::cvtColor(m_raw_image, grayImage, cv::COLOR_BGR2GRAY);

    m_raw_image = grayImage;
}

int ImageProcessor::flipped_grayscale(const std::string &output_path)
{
    cv::Mat flipped_img;
    cv::flip(m_raw_image, flipped_img, 1);
    cv::Mat gray_img;
    cv::cvtColor(flipped_img, gray_img, cv::COLOR_BGR2GRAY);
    return cv::imwrite(output_path, gray_img) ? 0 : -1;
}

pybind11::array_t<unsigned char> ImageProcessor::get_image()
{
    cv2eigen(m_raw_image, m_eigen_image);
    return pybind11::array_t<unsigned char>({m_eigen_image.rows(), m_eigen_image.cols()}, m_eigen_image.data());
}

void ImageProcessor::write(const std::string &output_path, const std::string &format)
{
    std::vector<int> compression_params;
    if (format == "jpg" || format == "jpeg")
    {
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(95);
    }
    else if (format == "png")
    {
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(3);
    }

    if (!cv::imwrite(output_path, m_raw_image, compression_params))
    {
        throw std::runtime_error("Failed to write image to file.");
    }
}

