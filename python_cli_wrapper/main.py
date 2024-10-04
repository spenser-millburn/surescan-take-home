import os
import logging
from concurrent.futures import ThreadPoolExecutor
import glob
from pathlib import Path
from image_transformers import flipped_grayscale
import typer

# Initialize logger
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

TRANSFORMATION_ALGORITHMS = {
    "flipped_grayscale": flipped_grayscale
    #... add more transformations here
}

def transform(image_path: Path, output_dir: Path, transformation_type: str):
    """transform a single image."""
    src = image_path.as_posix()
    name, ext = os.path.splitext(image_path.name)
    dest = (output_dir / f"{name}_{transformation_type}{ext}").resolve().as_posix()
    logger.info(f"transforming {image_path} with transformation [{transformation_type}] to {output_dir}/{image_path}")
    TRANSFORMATION_ALGORITHMS[transformation_type](src, dest)

def find_images(input_dir):
    """use glob to find all images in the input directory"""
    image_paths = glob.glob(os.path.join(input_dir, '*'))

    if not image_paths:
        logger.error("no images found in the input directory.")
        exit(1)
    return [Path(path) for path in image_paths]

def transform_images(transformation_type: str, input_dir: str, output_dir: str):
    """
    transform images from the input directory and save them to the output directory.
    """
    input_dir_path = Path(input_dir).resolve()
    output_dir_path = (Path(output_dir) / 'images').resolve()

    if transformation_type not in TRANSFORMATION_ALGORITHMS.keys():
        logger.critical(f"invalid transformation \"{transformation_type}\", Options: {TRANSFORMATION_ALGORITHMS.keys()} ")
        exit(1)

    if not input_dir_path.exists():
        logger.critical(f"Input directory {input_dir_path}: does not exist, cwd: {os.getcwd()}. Aborting...")
        exit(1)

    if not output_dir_path.exists():
        logger.warning(f"Output directory {output_dir_path}: does not exist {os.getcwd()}. creating...")
        output_dir_path.mkdir(parents=True, exist_ok=True)

    image_paths = find_images(input_dir)

    logger.info("Transforming images...")

    # Use ThreadPoolExecutor to transform images concurrently
    with ThreadPoolExecutor() as executor:
        futures = [executor.submit(transform, image_path, output_dir_path, transformation_type) for image_path in image_paths]
        for future in futures:
            future.result()  # Wait for each transformation to complete

    logger.info("Image transformation completed.")

app = typer.Typer()

@app.command()
def main(
    transformation_type: str = typer.Option(..., help="Type of transformation to apply"),
    input_dir: str = typer.Option(..., help="Directory containing input images"),
    output_dir: str = typer.Option(..., help="Directory to save transformed images")
):
    transform_images(transformation_type, input_dir, output_dir)

if __name__ == "__main__":
    app()
