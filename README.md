# SureScan Takehome Project - Spenser Millburn

## Overview
This project is a takehome assignment for SureScan. The purpose of the app is to apply image processing/transformations.

## Architecture

- **Core Transformation Logic**: Implemented as a C++ library.
  - Debian package artifacts are published via CI, see Actions tab of this repo.
- **APIs**: The core c++ [eigen/opencv] based library Exposed to both Python and C++ bindings.
- **Clients**: Desktop, CLI, and web (via rest API) clients wrap the library and are built and run as Docker Compose services.

### UML
![](./docs/assets/Architecture.svg)
![](./docs/assets/ImageProcessor.svg)

## Usage

### Quickstart - Launch ALL Clients
- Ensure Docker Compose is installed.
- The desktop client uses X Forwarding, so you may need to run `xhost +`.

```
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
  - Browse to [http://localhost:3000](http://localhost:3000) and follow on-screen prompts. You may either upload a single image or a tarball of images.

- **REST API**:
  - See RESTful API documentation at [http://localhost:8000/docs](http://localhost:8000/docs).

## Additional Information
- Built with love by Spenser Millburn.
