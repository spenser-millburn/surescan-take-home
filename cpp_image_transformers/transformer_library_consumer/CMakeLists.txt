# Require CMake 3.15+
cmake_minimum_required(VERSION 3.15)

# Project name and C++ standard
project(AnotherProject LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the ImageProcessor package (the library we installed previously)
find_package(ImageProcessor REQUIRED)

# Create an executable for this project
add_executable(another_project src/main.cpp)

# Link the ImageProcessor library and other dependencies (OpenCV, Python3, Eigen3) to this executable
target_link_libraries(another_project PRIVATE 
    ImageProcessor::image_processor_lib 
    ${OpenCV_LIBS}
    ${Python3_LIBRARIES}
    ${LLVM_LIBS}
    Eigen3::Eigen  # Link Eigen3
)

