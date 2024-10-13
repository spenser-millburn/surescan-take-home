import typer
from typing import List
from codetiming import Timer
from image_transformer_python_wrapper import transform_images, TRANSFORMATION_ALGORITHMS

app = typer.Typer()

@app.command()
def main(
    transformation_types: List[str] = typer.Option(..., "--transformation", help=f"Transformation(s) to apply, options are {TRANSFORMATION_ALGORITHMS}. NOTE: you may pass multiple transformations like --transformation flip_x --transformation grayscale"),
    input_dir: str = typer.Option(..., help="Directory containing input images"),
    output_dir: str = typer.Option(default="./output", help="Directory to save transformed images")
):
    with Timer(text="Function transform_images executed in {seconds:.5f} seconds"):
        transform_images(transformation_types, input_dir, output_dir)

if __name__ == "__main__":
    app()
