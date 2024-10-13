# SureScan Takehome Project - Spenser Millburn

## Overview
This project is a takehome assignment for SureScan. The purpose of the app is to apply image processing/transformations.

## Architecture

- **Core Transformation Logic**: Implemented as a C++ library. Debian package artifacts are published via CI for installation.
- **APIs**: The core c++ [eigen/opencv] based library Exposed to both Python and C++ bindings.
- **Clients**: Desktop, CLI, and web (via rest API) clients wrap the library and are built and run as Docker Compose services.

### UML
<img src=./docs/assets/Architecture.svg" alt="Description of the image">

## Usage

### Quickstart - Launch ALL Clients
- Ensure Docker Compose is installed.
- The desktop client uses X Forwarding, so you may need to run `xhost +`.

```shell
docker compose up --build
```

### Launching and Using Individual Services

- **DESKTOP C++ QT5 GUI**: 
  - Should pop up on the screen. Select an image and follow the on-screen prompts.
  - Command: `docker compose run -it qt_gui_wrapper`

- **Python Terminal UI**: 
  - Automatically processes images placed in the specified input directory, applying the defined transformations and saving the results to the output directory.
  - Command: 
    ```shell
    docker compose run -it python_tui bash -c "python3 /app/python_wrappers/cli.py --input-dir ./images/ --transformation flip_x_axis --transformation grayscale --output-dir ./output"
    ```
  - The transformed images appear in the `./output` directory on the host.

- **Typescript React Web GUI**: 
  - Browse to `http://localhost:3000` and follow on-screen prompts. You may either upload a single image or a tarball of images.

- **REST API**: 
  - See RESTful API documentation at `http://localhost:8000/docs`.

## Components

1. **Qt GUI Application**: 
   - A desktop application built using Qt that allows users to browse and display images. It is designed to be simple and intuitive, providing basic image viewing capabilities.

2. **Python Typer Application**: 
   - A command-line interface built with Typer that automates image processing tasks. It uses a C++ backend for performing image transformations such as flipping and converting images to grayscale.

3. **C++ Backend**: 
   - Utilizes OpenCV and Eigen3 libraries to perform image transformations. The backend is exposed to Python via Pybind11.

   ```shell
   docker-compose run --rm python_wrappers python3 python_wrappers/main.py --input-dir python_wrappers/images/ --transformation-type flipped_grayscale --output-dir ./python_wrappers/output
   ```
   - The transformed images appear in the output directory.

4. **Start the QT GUI**: 
   - Ensure you have X_FORWARDING set up.
   ```shell
   docker-compose up --build qt_gui
   ```
   - Follow the on-screen prompts.

## Additional Information

- Built with love by Spenser Millburn.
