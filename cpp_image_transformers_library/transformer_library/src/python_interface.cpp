#include <pybind11/pybind11.h>
#include "image_processor.h"

PYBIND11_MODULE(_core, m) {
    pybind11::class_<Processor>(m, "Processor");
    pybind11::class_<ImageProcessor, Processor>(m, "ImageProcessor")
        .def(pybind11::init<const std::string &>())
        .def(pybind11::init<>())
        .def("reset_image", &ImageProcessor::reset_image, "Reset the image to its original format :wq")
        .def("get_average_pixel_density", &ImageProcessor::get_average_pixel_density, "get the average pixel density of the image")
        .def("flipped_grayscale", &ImageProcessor::flipped_grayscale, "A function that transforms (flips) an image and saves it to the output path.")
        .def("flip_x", &ImageProcessor::flip_x, "Flip image along x-axis.")
        .def("flip_y", &ImageProcessor::flip_y, "Flip image along y-axis.")
        .def("rotate_left", &ImageProcessor::rotate_left, "Rotate image 90 degrees to the left.")
        .def("rotate_right", &ImageProcessor::rotate_right, "Rotate image 90 degrees to the right.")
        .def("grayscale", &ImageProcessor::grayscale, "Convert image to grayscale.")
        .def("read", &ImageProcessor::read, "Set the image being processed by reading it into memory")
        .def("write", &ImageProcessor::write, "Write the image to a file of specified format. ")
        .def("get_image", &ImageProcessor::get_image, "Return the Eigen matrix of the image.")
        .def("apply_transformation", &ImageProcessor::apply_transformation, "apply a transformation from string name")
        .def("get_transformations", &ImageProcessor::get_transformations, "Return a list of possible image transformations. ");


    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: transformer librayr 

        .. autosummary::
           :toctree: _generate

           ImageProcessor
    )pbdoc";

    m.attr("__version__") = "dev";
}
