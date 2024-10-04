FROM python:3.11-bullseye

# DEPS
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libopencv-dev \
    libeigen3-dev \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*
RUN pip3 install typer

#IMAGE TRANSFORMATIONS
WORKDIR /app
COPY ./cpp_image_transformers ./cpp_image_transformers 
RUN pip3 install ./cpp_image_transformers
WORKDIR /app/cpp_image_transformers
# CMD ["python3", "-c", "import image_transformers; image_transformers.flipped_grayscale('./cat.jpg','./output.jpg')"]

#Typer CLI Wrapper
WORKDIR /app
COPY ./python_cli_wrapper ./python_cli_wrapper
WORKDIR /app/python_cli_wrapper
CMD ["python3", "main.py", "--input-dir", "./images/", "--transformation-type", "flipped_grayscale", "--output-dir", "./output"]

