# ---- Require CMake 3.15+ ----
cmake_minimum_required(VERSION 3.15)

# ---- Project name and C++ standard ----
project(ImageProcessor VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(GTEST_OUTPUT "xml:unit_test_results.xml")


# ---- Set install prefix ----
set(CMAKE_INSTALL_PREFIX "/")
set(CPACK_PACKAGING_INSTALL_PREFIX "/")

# ---- Find dependencies ----
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(LLVM REQUIRED CONFIG)
find_package(Python3 REQUIRED COMPONENTS Development)
find_package(pybind11 CONFIG REQUIRED)

# ---- Include directories for dependencies ----
include_directories(${LLVM_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS} ${pybind11_INCLUDE_DIRS} /usr/local/lib/python3.8/dist-packages/pybind11/include/pybind11/)

message(STATUS "Building Image Processing Libs")

# ---- Build the C++ shared library ----
add_library(image_processor_lib SHARED src/image_processor.cpp)

# Set the include directories using BUILD_INTERFACE for build, and INSTALL_INTERFACE for installation
target_include_directories(image_processor_lib
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

# Link required libraries
target_link_libraries(image_processor_lib PRIVATE ${OpenCV_LIBS} Eigen3::Eigen ${LLVM_LIBS} Python3::Python)

# ---- Build the C++ executable ----
add_executable(image_processor_demo src/example.cpp)

target_link_libraries(image_processor_demo PRIVATE image_processor_lib ${OpenCV_LIBS} Eigen3::Eigen ${LLVM_LIBS} Python3::Python)

enable_testing()
find_package(GTest REQUIRED)

# Add the unit test executable
add_executable(unit_tests test/unit_tests.cpp src/image_processor.cpp)
target_include_directories(unit_tests
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)
target_link_libraries(unit_tests GTest::gtest GTest::gtest_main image_processor_lib ${OpenCV_LIBS} Eigen3::Eigen ${LLVM_LIBS} Python3::Python )

# Add a test
# add_test(NAME MyUnitTests COMMAND UnitTests)


# Set output directories for binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# ---- Installation rules for the library and executable ----
install(TARGETS image_processor_lib image_processor_demo
  EXPORT ImageProcessorTargets
  LIBRARY DESTINATION lib  # Install shared library to lib/
  RUNTIME DESTINATION bin  # Install executable to bin/
)

# Install include files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
)

# ---- Install CMake package configuration files ----
include(CMakePackageConfigHelpers)

# Configure the CMake package config file and version file
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ImageProcessorConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/ImageProcessorConfig.cmake"
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/ImageProcessorConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# Export targets to be used by other CMake projects
export(EXPORT ImageProcessorTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/ImageProcessorTargets.cmake"
  NAMESPACE ImageProcessor::
)

# Install the CMake config, version, and target files
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/ImageProcessorConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/ImageProcessorConfigVersion.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/ImageProcessorTargets.cmake"
  DESTINATION lib/cmake/${PROJECT_NAME}
)

# ---- CPack Configuration for Debian Package (.deb) ----

# Package name, version, and maintainer
set(CPACK_PACKAGE_NAME "image-processor-lib")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "spenser.millburn@surescan.com>")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Spenser")  # Maintainer field
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Image processing library with demo")
set(CPACK_PACKAGE_VENDOR "Surescan")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "ImageProcessorLibrary")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libopencv-dev, libeigen3-dev, python3-dev, pybind11-dev")

# Automatically add shared library dependencies
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# Generate a .deb package
set(CPACK_GENERATOR "DEB")

INCLUDE(CPack)
