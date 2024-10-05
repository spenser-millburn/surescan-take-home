# SureScan Takehome Project - Spenser Millburn

## Overview
This project is a takehome assignment for SureScan. The purpose of the contained application is to apply image processing/transformations.

## Usage
- **GUI **: Once the Docker services are up, you can interact with the Qt GUI to browse and display images.

      - TODO add image

- **CLI**: The CLI interface will automatically process images placed in the specified input directory, applying the defined transformations and saving the results to a specified output directory.

```
[N] ~/e/r/s/s/python_cli_wrapper ❯❯❯ docker-compose run --rm python_cli_wrapper python3 python_cli_wrapper/main.py  --help
                                                                                                                                                                     
 Usage: main.py [OPTIONS]              
                                                                                                                                                                     
╭─ Options ─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
│ *  --transformation-type        TEXT  Type of transformation to apply [default: None] [required]                                                                  │
│ *  --input-dir                  TEXT  Directory containing input images [default: None] [required]                                                                │
│ *  --output-dir                 TEXT  Directory to save transformed images [default: None] [required]                                                             │
│    --install-completion               Install completion for the current shell.                                                                                   │
│    --show-completion                  Show completion for the current shell, to copy it or customize the installation.                                            │
│    --help                             Show this message and exit.                                                                                                 │
╰───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯
```

## Components
1. **Qt GUI Application**: A desktop application built using Qt that allows users to browse and display images. It is designed to be simple and intuitive, providing basic image viewing capabilities.

2. **Python Typer Application**: A command-line interface built with Typer that automates image processing tasks. It uses a C++ backend for performing image transformations such as flipping and converting images to grayscale.

3. **C++ Backend**: Utilizes OpenCV and Eigen3 libraries to perform image transformations. The backend is exposed to Python via Pybind11, allowing seamless integration with the Python.

## Getting Started
To run the project, ensure you have Docker and Docker Compose installed on your system. Follow these steps:

1. Clone the repository to your local machine.
2. Navigate to the project directory.
3. Run the following command to transform the demo images, the output will appear in ./output:
  - Bonus: I have included a few other types of transformations, see the --help option of the cli for details. 

```
docker-compose run --rm python_cli_wrapper python3 python_cli_wrapper/main.py --input-dir python_cli_wrapper/images/ --transformation-type flipped_grayscale --output-dir ./python_cli_wrapper/output
```
4. Start the QT GUI 
  - NOTE: make sure you have X_FORWARDING set up. 
```
docker-compose up --build qt_gui
```

The transformed images appear in the output directory. Alertnatively



## Conclusion
This project demonstrates the integration of a Qt-based GUI with a Python CLI application that uses a C++ backend for image processing
