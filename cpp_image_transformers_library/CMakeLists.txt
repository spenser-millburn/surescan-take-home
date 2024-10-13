cmake_minimum_required(VERSION 3.15...3.27)

project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

#find opencv and eigen
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)

# Find the module development requirements (requires FindPython from 3.17 or
# scikit-build-core's built-in backport)
find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/transformer_library/include)

# Add a library using FindPython's tooling
python_add_library(_core MODULE
  transformer_library/src/image_processor.cpp
  transformer_library/src/python_interface.cpp
  WITH_SOABI)


target_link_libraries(_core PRIVATE ${OpenCV_LIBS} Eigen3::Eigen pybind11::headers )

target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

install(TARGETS _core DESTINATION image_transformers)
install(FILES ${CMAKE_SOURCE_DIR}/transformer_library/image_transformers/__init__.py DESTINATION image_transformers)

