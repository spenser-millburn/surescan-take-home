# Require CMake 3.15+
cmake_minimum_required(VERSION 3.15)

# Project name and C++ standard
project(ImageProcessorGUI LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find packages
find_package(Qt5 COMPONENTS Widgets REQUIRED)  # Find the Qt5 Widgets component
find_package(ImageProcessor REQUIRED)         # Find the custom ImageProcessor package

# Ensure automatic handling of Qt meta-object compiler, UI compiler, and resource compiler
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Set include directories for Qt5 and ImageProcessor
target_include_directories(${PROJECT_NAME} PRIVATE ${Qt5Widgets_INCLUDE_DIRS})

# Add necessary Qt definitions and flags
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# Link libraries: Qt5, ImageProcessor, OpenCV, Python3, Eigen3
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt5::Widgets  
    ImageProcessor::image_processor_lib
    ${OpenCV_LIBS}
    ${Python3_LIBRARIES}
    Eigen3::Eigen  # Link Eigen3
)

# Specify the RPATH to ensure the executable finds the ImageProcessor shared library
set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"  # Adjust the RPATH based on where the ImageProcessor library is installed
)

# Specify installation directory and install the executable
install(TARGETS ${PROJECT_NAME} 
    RUNTIME DESTINATION bin  # Install the binary in the 'bin' directory
)

# Optionally, copy the shared library from ImageProcessor to the appropriate location (for example, 'lib')
install(FILES $<TARGET_FILE:ImageProcessor::image_processor_lib> DESTINATION lib)
