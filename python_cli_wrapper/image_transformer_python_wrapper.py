import os
from concurrent.futures import ThreadPoolExecutor
import glob
from pathlib import Path
from typing import List
from image_transformers import ImageProcessor
import logging
# Initialize logger
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)
surescan_processor = ImageProcessor()

TRANSFORMATION_ALGORITHMS = surescan_processor.get_transformations()
def transform(image_path: Path, output_dir: Path, transformation_types: List[str]):
    """Transforms a single image with a sequence of transformations applied to the same image."""
    src = image_path.as_posix()
    name, ext = os.path.splitext(image_path.name)
    dest = (output_dir / f"{name}_{'_'.join(transformation_types)}{ext}").resolve().as_posix()
    logger.info(f"Transforming {image_path} with transformations {transformation_types} to {dest}")

    # Load the image into the processors
    surescan_processor = ImageProcessor(src)

    # Apply all transformations
    for transformation_type in transformation_types:
        try:
            logger.info(f"Applying transformation: {transformation_type}")
            surescan_processor.apply_transformation(transformation_type)
        except Exception as e:
            logger.warning(f"Failed to apply transformation: {transformation_type}. Error: {e}")

    # Write image to destination
    surescan_processor.write_image(dest, ext)

def find_images(input_dir: str) -> List[Path]:
    """Globs image files in the input directory and returns a list of paths."""
    supported_image_extensions = ('*.jpg', '*.jpeg', '*.png')  # Add other extensions if needed
    image_paths = []

    for ext in supported_image_extensions:
        image_paths.extend(glob.glob(os.path.join(input_dir, '**', ext), recursive=True))

    if not image_paths:
        logger.error("No images found in the input directory.")
        exit(1)

    return [Path(path) for path in image_paths]

def transform_images(transformation_types: List[str], input_dir: str, output_dir: str):
    """
    Transforms images from the input directory and saves them to the output directory using multiple transformations.
    """
    input_dir_path = Path(input_dir).resolve()
    output_dir_path = (Path(output_dir) / 'images').resolve()

    # Validation
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

    # Transform images concurrently
    with ThreadPoolExecutor() as executor:
        futures = []
        for image_path in image_paths:
            futures.append(executor.submit(transform, image_path, output_dir_path, transformation_types))
        
        for future in futures:
            future.result()

    logger.info("Image transformation completed.")
