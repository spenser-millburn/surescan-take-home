#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <string>
#include <map>
#include <functional>

class Processor {
public:
    virtual ~Processor() = default;
    virtual void read(const std::string &path) = 0;
    virtual void write(const std::string &output_path, const std::string &format) = 0;
};

class ImageProcessor : public Processor {
public:
    std::string m_raw_image_path;
    cv::Mat m_raw_image;
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> m_eigen_image;
    void cv2eigen(const cv::Mat& src, Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& dst);
    void eigen2cv(const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& src, cv::Mat& dst);
    ImageProcessor(const std::string &image_path);
    ImageProcessor(){};
    double get_average_pixel_density();
    void reset_image();
    void flip_x();
    void flip_y();
    void rotate_left();
    void rotate_right();
    void grayscale();
    pybind11::array_t<unsigned char> get_image();

    std::map<std::string, std::function<void()>> m_transformations_map =
    {
        {"reset_image", [this]() { reset_image(); }},
        {"flip_x_axis", [this]() { flip_x(); }},
        {"flip_y_axis", [this]() { flip_y(); }},
        {"rotate_left", [this]() { rotate_left(); }},
        {"rotate_right", [this]() { rotate_right(); }},
        {"grayscale", [this]() { grayscale(); }}
    };

    pybind11::list get_transformations()
    {
        pybind11::list l_py_list;

        // Populate the py::list with elements from the map
        for (const auto& func : m_transformations_map) {
            l_py_list.append(func.first); // Append the transformation name (key)
        }

        return l_py_list;
    }

    void apply_transformation(const std::string &transformation_name)
    {
        auto it = m_transformations_map.find(transformation_name);
        if (it != m_transformations_map.end()) {
            it->second();
        } else {
            throw std::invalid_argument("Invalid transformation: " + transformation_name);
        }
    }

    int flipped_grayscale(const std::string &output_path);
    void read(const std::string &image_path) override;
    void write(const std::string &output_path, const std::string &format) override;
};

#endif // IMAGE_PROCESSOR_H
