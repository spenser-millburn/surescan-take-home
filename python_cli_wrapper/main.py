import functools
import os
import logging
from concurrent.futures import ThreadPoolExecutor
import glob
from pathlib import Path
import time
from image_transformers import ImageProcessor
import typer
from typing import List
from codetiming import Timer


# Initialize logger
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

TRANSFORMATION_ALGORITHMS = ImageProcessor().get_transformations()

def transform(image_path: Path, output_dir: Path, transformation_types: List[str]):
    """transforms a single image with a sequence of transformations applied to the same image."""
    src = image_path.as_posix()
    name, ext = os.path.splitext(image_path.name)
    dest = (output_dir / f"{name}_{'_'.join(transformation_types)}{ext}").resolve().as_posix()
    logger.info(f"Transforming {image_path} with transformations {transformation_types} to {dest}")

    # load
    im = ImageProcessor(src)

    # apply all transformatiosn
    for transformation_type in transformation_types:
        logger.info(f"Applying transformation: {transformation_type}")
        im.apply_transformation(transformation_type)

    # write image to dest
    im.write_image(dest, ext)

def find_images(input_dir):
    """globs images in input dir and returns a list of paths"""
    image_paths = glob.glob(os.path.join(input_dir, '*'))

    if not image_paths:
        logger.error("No images found in the input directory.")
        exit(1)
    return [Path(path) for path in image_paths]



@Timer(text="Function transform_images executed in {seconds:.5f} seconds")
def transform_images(transformation_types: List[str], input_dir: str, output_dir: str):
    """
    transform images from the input directory and save them to the output directory using multiple transformations.
    """
    input_dir_path = Path(input_dir).resolve()
    output_dir_path = (Path(output_dir) / 'images').resolve()

    #validation
    for transformation_type in transformation_types:
        if transformation_type not in TRANSFORMATION_ALGORITHMS:
            logger.critical(f"Invalid transformation \"{transformation_type}\", options: {TRANSFORMATION_ALGORITHMS}")
            exit(1)

    if not input_dir_path.exists():
        logger.critical(f"Input directory {input_dir_path}: does not exist, cwd: {os.getcwd()}. Aborting...")
        exit(1)

    if not output_dir_path.exists():
        logger.warning(f"Output directory {output_dir_path}: does not exist. Creating...")
        output_dir_path.mkdir(parents=True, exist_ok=True)

    image_paths = find_images(input_dir)

    logger.info("Transforming images...")

    # transform images concurrently
    with ThreadPoolExecutor() as executor:
        futures = []
        for image_path in image_paths:
            futures.append(executor.submit(transform, image_path, output_dir_path, transformation_types))
        
        for future in futures:
            future.result() 

    logger.info("Image transformation completed.")

app = typer.Typer()

@app.command()
def main(
    transformation_types: List[str] = typer.Option(..., "--transformation", help=f"Transformation(s) to apply, options are {TRANSFORMATION_ALGORITHMS} NOTE: you may pass multiple transformations like --transformation flip_x --transformation grayscale"),
    input_dir: str = typer.Option(..., help="Directory containing input images"),
    output_dir: str = typer.Option(default="./output", help="Directory to save transformed images")
):
    transform_images(transformation_types, input_dir, output_dir)

if __name__ == "__main__":
    app()
