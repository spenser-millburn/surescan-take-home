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

message(STATUS "Building Image Processing Libs")

# Build the C++ shared library
add_library(image_processor_lib SHARED src/image_processor.cpp)

# Set the include directories using BUILD_INTERFACE for build, and INSTALL_INTERFACE for installation
target_include_directories(image_processor_lib
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_link_libraries(image_processor_lib PRIVATE ${OpenCV_LIBS} Eigen3::Eigen ${LLVM_LIBS} Python3::Python)

# Build the C++ executable
add_executable(image_processor_demo src/example.cpp)
target_link_libraries(image_processor_demo PRIVATE image_processor_lib ${OpenCV_LIBS} Eigen3::Eigen ${LLVM_LIBS} Python3::Python)

# Set output directories
set_target_properties(image_processor_demo PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set_target_properties(image_processor_demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Installation rules for the library and executable
install(TARGETS image_processor_lib image_processor_demo
  EXPORT ImageProcessorTargets  # This exports the targets
  LIBRARY DESTINATION lib/${PROJECT_NAME}
  RUNTIME DESTINATION bin
)

# Install include files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
)

# Create version and config files for the library
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/ImageProcessorConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ImageProcessorConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/ImageProcessorConfig.cmake"
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

# Install the config and version files
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/ImageProcessorConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/ImageProcessorConfigVersion.cmake"
  DESTINATION lib/cmake/${PROJECT_NAME}
)

# Export the targets for other projects to link
install(EXPORT ImageProcessorTargets
  FILE ImageProcessorTargets.cmake
  NAMESPACE ImageProcessor::
  DESTINATION lib/cmake/${PROJECT_NAME}
)
