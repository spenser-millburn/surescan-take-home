FROM ubuntu:20.04

# DEPS
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y build-essential python3 python3-pip 
RUN apt-get install -y cmake libopencv-dev libeigen3-dev
RUN apt-get install -y llvm qt5-default python3-pybind11
RUN apt-get install -y libgtest-dev

# RUN rm -rf /var/lib/apt/lists/*
RUN pip3 install typer fastapi codetiming pybind11 python-multipart uvicorn pytest



#Transformations Library - Python API install
WORKDIR /app
COPY ./cpp_image_transformers_library ./cpp_image_transformers_library
RUN pip3 install ./cpp_image_transformers_library

#Transformations Library - CPP Library Install
WORKDIR /app/cpp_image_transformers_library/transformer_library
SHELL ["/bin/bash", "-c"]
RUN python3 -m pybind11 --include
RUN rm -rf build && mkdir -p build && cd build && cmake .. && make package install
RUN mkdir -p /output && cp ./build/*.deb /output

# Building the Desktop GUI executable 
WORKDIR /app
COPY ./qt_gui_wrapper ./qt_gui_wrapper
WORKDIR /app/qt_gui_wrapper
RUN rm -rf build && mkdir -p build && cd build && cmake .. && make package install 
RUN mkdir -p /output && cp ./build/*.deb /output
WORKDIR /app/cpp_image_transformers_library

#Typer CLI Wrapper
WORKDIR /app
COPY ./python_wrappers ./python_wrappers
COPY ./images ./images
WORKDIR /app/python_wrappers
CMD ["python3", "main.py", "--input-dir", "./images/", "--transformation", "flip_x_axis", "--output-dir", "./output"]
# CMD ["bash"]

