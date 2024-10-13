# CMake minimum version
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
set(CPACK_INSTALL_PREFIX "/")
# Specify the RPATH to ensure the executable finds the ImageProcessor shared library
set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "/lib"  # Adjust the RPATH based on where the ImageProcessor library is installed
)

set(CMAKE_PREFIX_PATH "/")
# Specify installation directory and install the executable
# ---- CPack Configuration for Debian Package (.deb) ----

# Set output directories for binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")


set(CPACK_PACKAGE_NAME "image-processor-gui")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "Your Name <your.email@example.com>")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name")  # Maintainer field for .deb
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A GUI application for image processing using Qt5 and ImageProcessor.")
set(CPACK_PACKAGE_VENDOR "Your Company Name")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "ImageProcessorGUI")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt5-default, libopencv-dev, python3, libeigen3-dev")

# Set the license file if any
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

# Set additional packaging details for the Debian package
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)  # Automatically add shared library dependencies
set(CPACK_GENERATOR "DEB")

install(TARGETS ${PROJECT_NAME} 
    RUNTIME DESTINATION bin  # Install the binary in the 'bin' directory
    LIBRARY DESTINATION lib   # Install shared libraries in 'lib'
    ARCHIVE DESTINATION lib   # Install static libraries in 'lib'
)

# Optionally, copy the shared library from ImageProcessor to the appropriate location (for example, 'lib')
install(FILES $<TARGET_FILE:ImageProcessor::image_processor_lib> DESTINATION lib)


# Specify the install prefix for the package
set(CPACK_PACKAGE_INSTALL_PREFIX "/")


# Include the package information in the build process
include(CPack)
