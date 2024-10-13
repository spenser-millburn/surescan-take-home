xhost +

docker compose up --build

docker compose run -it qt_gui_wrapper

docker compose run -it python_tui bash -c "python3 /app/python_wrappers/cli.py --input-dir ./images/ --transformation flip_x_axis --transformation grayscale --output-dir ./output"

docker compose run -it python_tui bash -c "python3 /app/python_wrappers/cli.py --help"

docker-compose run --rm python_wrappers python3 python_wrappers/main.py --input-dir python_wrappers/images/ --transformation-type flipped_grayscale --output-dir ./python_wrappers/output

docker-compose up --build qt_gui
