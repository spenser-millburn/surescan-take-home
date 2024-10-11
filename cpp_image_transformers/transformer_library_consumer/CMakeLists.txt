# Require CMake 3.15+
cmake_minimum_required(VERSION 3.15)

# Project name and C++ standard
project(AnotherProject LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find packages
find_package(Qt5 COMPONENTS Widgets REQUIRED)  # Find the Qt5 Widgets component
find_package(ImageProcessor REQUIRED)         # Assuming this is a custom package you have

# Include Qt5 for CMake's AUTOMOC and AUTOUIC if your project needs it
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add the executable for this project
add_executable(another_project src/main.cpp)

# Include directories for Qt5
target_include_directories(another_project PRIVATE ${Qt5Widgets_INCLUDE_DIRS})

# Define necessary definitions and compile options for Qt
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# Link the libraries
target_link_libraries(another_project PRIVATE 
    Qt5::Widgets  # Link against Qt5 Widgets
    ImageProcessor::image_processor_lib
    ${OpenCV_LIBS}
    ${Python3_LIBRARIES}
    Eigen3::Eigen  # Link Eigen3
)
