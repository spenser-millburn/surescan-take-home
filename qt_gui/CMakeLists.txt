# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MyImageBrowser)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the Qt5 package
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Add the executable
add_executable(MyImageBrowser main.cpp)

# Link the executable with the Qt5 Widgets library
target_link_libraries(MyImageBrowser Qt5::Widgets)
