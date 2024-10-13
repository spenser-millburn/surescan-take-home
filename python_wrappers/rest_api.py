import glob
import tarfile
import shutil
from fastapi import FastAPI, HTTPException, UploadFile, File
from typing import List
from surescan_image_transformer_core_wrapper import transform_images,find_images,  TRANSFORMATION_ALGORITHMS
from fastapi.responses import FileResponse
import os
import logging
import uuid

# Initialize logger
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

# Initialize api
app = FastAPI()

# Define fixed temp directories for inputs and outputs
UPLOAD_DIR = os.path.abspath("/tmp/uploads")
PROCESSED_DIR = os.path.abspath("/tmp/processed")

# Ensure directories exist
os.makedirs(UPLOAD_DIR, exist_ok=True)
os.makedirs(PROCESSED_DIR, exist_ok=True)

# List of valid image extensions
VALID_IMAGE_EXTENSIONS = ('.jpg', '.jpeg', '.png')

def create_unique_dirs():
    """Creates unique input/output directories based on UUID."""
    unique_id = str(uuid.uuid4())
    input_dir = os.path.join(UPLOAD_DIR, unique_id)
    output_dir = os.path.join(PROCESSED_DIR, unique_id)
    os.makedirs(input_dir, exist_ok=True)
    os.makedirs(output_dir, exist_ok=True)
    return input_dir, output_dir, unique_id

def save_uploaded_file(uploaded_file: UploadFile, destination_path: str):
    """Saves an uploaded file to the given destination."""
    with open(destination_path, "wb") as buffer:
        shutil.copyfileobj(uploaded_file.file, buffer)

def transform_and_package_images(transformation_types: List[str], input_dir: str, output_dir: str, unique_id: str):
    """Performs image transformations and packages the output as a tarball."""
    transform_images(
        transformation_types=transformation_types,
        input_dir=input_dir,
        output_dir=output_dir
    )

    output_tar_path = os.path.join(PROCESSED_DIR, f"{unique_id}_transformed_images.tar.gz")
    with tarfile.open(output_tar_path, "w:gz") as tar:
        tar.add(output_dir, arcname="transformed_images")

    return output_tar_path

@app.get("/transformations")
async def get_transformations():
    """Gets a list of available transformations from the surescan image processing library"""
    return TRANSFORMATION_ALGORITHMS


@app.post("/process_images")
async def process_images( transformation_types: List[str],  file: UploadFile = File(...)):
    """Processes a tarball of images into the transformed output, and returns a tarball with all images transformed. """
    try:
        input_dir, output_dir, unique_id = create_unique_dirs()

        # save tarball
        input_tar_path = os.path.join(input_dir, file.filename)
        save_uploaded_file(file, input_tar_path)

        #extract
        with tarfile.open(input_tar_path, "r:gz") as tar:
            tar.extractall(path=input_dir)

        # do image transformations and tarball up the result again
        output_tar_path = transform_and_package_images(transformation_types, input_dir, output_dir, unique_id)

        logger.info(f"created tarball at {output_tar_path} with size {os.path.getsize(output_tar_path)}, sending back to client as a FileResponse immediately.")

        return FileResponse(
            path=output_tar_path,
            filename="transformed_images.tar.gz",
            media_type="application/x-tar"
        )

    except Exception as e:
        logger.error(f"Error processing images: {str(e)}")
        raise HTTPException(status_code=500, detail=str(e))


@app.post("/process_single_image")
async def process_single_image(transformation_types: List[str],file: UploadFile = File(...)):
    """Processes a single images into the transformed output, and returns a the transformed image"""
    try:
        input_dir, output_dir, unique_id = create_unique_dirs()

        input_image_path = os.path.join(input_dir, file.filename)
        save_uploaded_file(file, input_image_path)

        # Check if the file has a valid image extension
        if not input_image_path.lower().endswith(VALID_IMAGE_EXTENSIONS):
            raise HTTPException(status_code=400, detail="Invalid image file format.")

        transform_images(
            transformation_types=transformation_types,
            input_dir=input_dir,
            output_dir=output_dir
        )


        output_image_path = find_images(output_dir)[0]
        logger.info(f"Processing complete for single image: {output_image_path}, sending back to client.")

        # Return the transformed image
        return FileResponse(output_image_path)

    except Exception as e:
        logger.error(f"Error processing single image: {str(e)}")
        raise HTTPException(status_code=500, detail=str(e))


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
