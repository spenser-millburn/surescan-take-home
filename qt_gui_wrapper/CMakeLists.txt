# Require CMake 3.15+
cmake_minimum_required(VERSION 3.15)

# Project name and C++ standard
project(ImageProcessorGUI LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find packages
find_package(Qt5 COMPONENTS Widgets REQUIRED)  # Find the Qt5 Widgets component
find_package(ImageProcessor REQUIRED)         # Assuming this is a custom package you have

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${Qt5Widgets_INCLUDE_DIRS})

add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt5::Widgets  
    ImageProcessor::image_processor_lib
    ${OpenCV_LIBS}
    ${Python3_LIBRARIES}
    Eigen3::Eigen  # Link Eigen3
)

