# SureScan Takehome Project - Spenser Millburn

## Overview
This project is a takehome assignment for SureScan. The purpose of the app is to apply image processing/transformations.

## Strategy
1. The core transformation logic is implemented as a C++ library. 
  - Debian package artifacts are published via CI should you like to install this core library.
2. APIs to the library are exposed to both python and C++ bindings
3. clients  (desktop , cli , web) wrap the library and are built and run as docker compose services

NOTE: This approach was taken so that client side code is does not need to be updated when new transformations or features are added to the core library. 

## Quickstart - Launch the clients
Make sure docker compose is installed. The desktop client uses X Forwarding , so you may need to run `xhost +` [https://www.ibm.com/docs/en/aix/7.1?topic=x-xhost-command](https://www.ibm.com/docs/en/aix/7.1?topic=x-xhost-command) 

```
docker compose up --build
```
Note that all services will be brought up at once. See below for details on usage

### Launching and using individual services 
- **DESKTOP C++ QT5 GUI**: Should pop up on the screen. Select an image, and follow the on screen prompts
  - `docker compose run -it qt_gui_wrapper`  

- **Python Terminal UI**: The CLI interface will automatically process images placed in the specified input directory, applying the defined transformations and saving the results to the output directory.
  - `docker compose run -it python_tui bash -c "python3 /app/python_wrappers/cli.py --input-dir ./images/ --transformation flip_x_axis --transformation grayscale --output-dir ./output"`
    - The transformed images appear in the ./output directory on the host.

```
[N] ~/e/r/s/s/python_wrappers ❯❯❯ docker compose run -it python_tui bash -c "python3 /app/python_wrappers/cli.py --help"
                                                                                                                 
 Usage: cli.py [OPTIONS]                                                                                         
                                                                                                                 
╭─ Options ─────────────────────────────────────────────────────────────────────────────────────────────────────╮
│ *  --transformation            TEXT  Transformation(s) to apply, options are ['flip_x_axis', 'flip_y_axis',   │
│                                      'grayscale', 'reset_image', 'rotate_left', 'rotate_right']. NOTE: you    │
│                                      may pass multiple transformations like --transformation flip_x           │
│                                      --transformation grayscale                                               │
│                                      [default: None]                                                          │
│                                      [required]                                                               │
│ *  --input-dir                 TEXT  Directory containing input images [default: None] [required]             │
│    --output-dir                TEXT  Directory to save transformed images [default: ./output]                 │
│    --install-completion              Install completion for the current shell.                                │
│    --show-completion                 Show completion for the current shell, to copy it or customize the       │
│                                      installation.                                                            │
│    --help                            Show this message and exit.                                              │
╰───────────────────────────────────────────────────────────────────────────────────────────────────────────────╯
```

- **Typescript React Web GUI** : Browse to [http://localhost:3000](http://localhost:3000) and follow on screen prompts. You may either upload a single image or a tarball of images. 

- **REST API** If you are interested, see restful api documentation at [http://localhost:8000/docs](http://localhost:8000/docs)

## Components
1. **Qt GUI Application**: A desktop application built using Qt that allows users to browse and display images. It is designed to be simple and intuitive, providing basic image viewing capabilities.

2. **Python Typer Application**: A command-line interface built with Typer that automates image processing tasks. It uses a C++ backend for performing image transformations such as flipping and converting images to grayscale.

3. **C++ Backend**: Utilizes OpenCV and Eigen3 libraries to perform image transformations. The backend is exposed to Python via Pybind11. 

```
docker-compose run --rm python_wrappers python3 python_wrappers/main.py --input-dir python_wrappers/images/ --transformation-type flipped_grayscale --output-dir ./python_wrappers/output
```
The transformed images appear in the output directory.

4. Start the QT GUI 
  - NOTE: make sure you have X_FORWARDING set up.
```
docker-compose up --build qt_gui
```
  - follow the on screen prompts. 

## Thank You
- built with love by spenser millburn


