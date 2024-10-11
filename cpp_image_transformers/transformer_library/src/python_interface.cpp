#include <pybind11/pybind11.h>
#include "image_processor.h"

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
        .def("read_image", &ImageProcessor::read_image, "Set the image being processed by reading it into memory")
        .def("write_image", &ImageProcessor::write_image, "Write the image to a file of specified format. ")
        .def("get_image", &ImageProcessor::get_image, "Return the Eigen matrix of the image.");

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
