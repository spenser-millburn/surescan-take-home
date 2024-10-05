# Require CMake 3.15+
cmake_minimum_required(VERSION 3.15)

# Project name and C++ standard
project(ImageProcessor VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find dependencies
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(LLVM REQUIRED CONFIG)
find_package(Python3 REQUIRED COMPONENTS Development)
find_package(pybind11 CONFIG REQUIRED)

# Include directories for dependencies
include_directories(${LLVM_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})


message(STATUS "Building C++ module")

# Build the C++ shared library
add_library(image_processor_lib SHARED image_processor.cpp)

target_link_libraries(image_processor_lib PRIVATE ${OpenCV_LIBS} Eigen3::Eigen ${LLVM_LIBS} Python3::Python)
target_include_directories(image_processor_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Build the C++ executable
add_executable(image_processor_demo main.cpp)
target_link_libraries(image_processor_demo PRIVATE image_processor_lib ${OpenCV_LIBS} Eigen3::Eigen ${LLVM_LIBS} Python3::Python)

# Set output directories
set_target_properties(image_processor_lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set_target_properties(image_processor_demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

