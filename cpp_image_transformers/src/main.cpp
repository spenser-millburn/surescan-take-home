#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <iostream>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

class Processor {
public:
    virtual ~Processor() = default;

protected:
    cv::Mat image;
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> eigenImage;

    void cv2eigen(const cv::Mat& src, Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& dst) {
        dst = Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(src.data, src.rows, src.cols);
    }

    void eigen2cv(const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& src, cv::Mat& dst) {
        dst = cv::Mat(src.rows(), src.cols(), CV_8UC1, const_cast<unsigned char*>(src.data()));
    }
};

class ImageProcessor : public Processor {
public:
    ImageProcessor(const std::string &image_path) {
        image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
        if (image.empty()) {
            throw std::runtime_error("Error: Could not open or find the image.");
        }
        cv2eigen(image, eigenImage);
    }

    pybind11::array_t<unsigned char> flip_x() {
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> flippedImage = eigenImage.rowwise().reverse();
        return pybind11::array_t<unsigned char>({flippedImage.rows(), flippedImage.cols()}, flippedImage.data());
    }

    pybind11::array_t<unsigned char> flip_y() {
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> flippedImage = eigenImage.colwise().reverse();
        return pybind11::array_t<unsigned char>({flippedImage.rows(), flippedImage.cols()}, flippedImage.data());
    }

    pybind11::array_t<unsigned char> rotate_left() {
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> rotatedImage = eigenImage.transpose().colwise().reverse();
        return pybind11::array_t<unsigned char>({rotatedImage.rows(), rotatedImage.cols()}, rotatedImage.data());
    }

    pybind11::array_t<unsigned char> rotate_right() {
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> rotatedImage = eigenImage.transpose().rowwise().reverse();
        return pybind11::array_t<unsigned char>({rotatedImage.rows(), rotatedImage.cols()}, rotatedImage.data());
    }

    pybind11::array_t<unsigned char> grayscale() {
        return pybind11::array_t<unsigned char>({eigenImage.rows(), eigenImage.cols()}, eigenImage.data());
    }

    pybind11::array_t<unsigned char> eigen_matrix() {
        return pybind11::array_t<unsigned char>({eigenImage.rows(), eigenImage.cols()}, eigenImage.data());
    }

    int flipped_grayscale(const std::string &output_path) {
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> flippedImage = eigenImage.colwise().reverse();
        cv::Mat flippedMat;
        eigen2cv(flippedImage, flippedMat);
        cv::imwrite(output_path, flippedMat);
        return 0; // Success
    }
};

PYBIND11_MODULE(_core, m) {
    pybind11::class_<Processor>(m, "Processor");
    pybind11::class_<ImageProcessor, Processor>(m, "ImageProcessor")
        .def(pybind11::init<const std::string &>())
        .def("flipped_grayscale", &ImageProcessor::flipped_grayscale, "A function that transforms (flips) an image and saves it to the output path.")
        .def("flip_x", &ImageProcessor::flip_x, "Flip image along x-axis.")
        .def("flip_y", &ImageProcessor::flip_y, "Flip image along y-axis.")
        .def("rotate_left", &ImageProcessor::rotate_left, "Rotate image 90 degrees to the left.")
        .def("rotate_right", &ImageProcessor::rotate_right, "Rotate image 90 degrees to the right.")
        .def("grayscale", &ImageProcessor::grayscale, "Convert image to grayscale.")
        .def("eigen_matrix", &ImageProcessor::eigen_matrix, "Return the Eigen matrix of the image.");

    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: scikit_build_example

        .. autosummary::
           :toctree: _generate

           ImageProcessor
    )pbdoc";

    m.attr("__version__") = "dev";
}
