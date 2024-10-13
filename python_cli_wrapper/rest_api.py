import tarfile
import shutil
from fastapi import FastAPI, HTTPException, UploadFile, File
from typing import List
from image_transformer_python_wrapper import transform_images
from fastapi.responses import FileResponse
import os
import logging
import uuid

# Initialize logger
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

app = FastAPI()

# Define fixed directories for inputs and outputs
UPLOAD_DIR = os.path.abspath("/tmp/uploads")
PROCESSED_DIR = os.path.abspath("/tmp/processed")

# Ensure directories exist
os.makedirs(UPLOAD_DIR, exist_ok=True)
os.makedirs(PROCESSED_DIR, exist_ok=True)

# List of valid image extensions
VALID_IMAGE_EXTENSIONS = ('.jpg', '.jpeg', '.png', '.bmp', '.gif', '.tiff', '.webp')

def is_image_file(filepath: str) -> bool:
    return filepath.lower().endswith(VALID_IMAGE_EXTENSIONS)

@app.post("/process_images")
async def process_images(
    transformation_types: List[str], 
    file: UploadFile = File(...)
):
    try:
        # generate a unique id to avoid conflicts in filenames
        unique_id = str(uuid.uuid4())
        input_dir = os.path.join(UPLOAD_DIR, unique_id)
        output_dir = os.path.join(PROCESSED_DIR, unique_id)
        
        os.makedirs(input_dir, exist_ok=True)
        os.makedirs(output_dir, exist_ok=True)

        input_tar_path = os.path.join(input_dir, file.filename)
        with open(input_tar_path, "wb") as buffer:
            shutil.copyfileobj(file.file, buffer)

        with tarfile.open(input_tar_path, "r:gz") as tar:
          tar.extractall(path=input_dir)

        transform_images(
            transformation_types=transformation_types, 
            input_dir=input_dir, 
            output_dir=output_dir
        )

        # Create the output tarball
        output_tar_path = os.path.join(PROCESSED_DIR, f"{unique_id}_transformed_images.tar.gz")
        with tarfile.open(output_tar_path, "w:gz") as tar:
            tar.add(output_dir, arcname="transformed_images")

        logger.info(f"Created tarball at {output_tar_path} with size {os.path.getsize(output_tar_path)}, sending back to client as a FileResponse immediately.")

        # Return the transformed tarball
        return FileResponse(
            path=output_tar_path,
            filename="transformed_images.tar.gz",
            media_type="application/x-tar"
        )

    except Exception as e:
        logger.error(f"error processing images: {str(e)}")
        raise HTTPException(status_code=500, detail=str(e))

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
