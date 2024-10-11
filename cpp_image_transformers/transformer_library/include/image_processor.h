#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <string>

class Processor {
public:
    virtual ~Processor() = default;

protected:
    cv::Mat raw_image;
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> eigenImage;

    void cv2eigen(const cv::Mat& src, Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& dst);
    void eigen2cv(const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& src, cv::Mat& dst);
};

class ImageProcessor : public Processor 
{
public:
    ImageProcessor(const std::string &image_path);
    pybind11::array_t<unsigned char> flip_x();
    pybind11::array_t<unsigned char> flip_y();
    pybind11::array_t<unsigned char> rotate_left();
    pybind11::array_t<unsigned char> rotate_right();
    pybind11::array_t<unsigned char> grayscale();
    pybind11::array_t<unsigned char> get_image();

    int flipped_grayscale(const std::string &output_path);
    void write_image(const std::string &output_path, const std::string &format);
};


#endif // IMAGE_PROCESSOR_H
