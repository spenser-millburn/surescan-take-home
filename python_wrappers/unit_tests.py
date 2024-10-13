import unittest
from unittest.mock import patch, MagicMock
from pathlib import Path
from surescan_image_transformer_core_wrapper import transform_images, find_images
import logging

# Initialize logger
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

class TestImageTransformer(unittest.TestCase):

    @patch('surescan_image_transformer_core_wrapper.ImageProcessor')
    def test_transform_images(self, MockImageProcessor):
        mock_processor = MockImageProcessor.return_value
        mock_processor.get_transformations.return_value = ['flip_x_axis', 'grayscale']
        mock_processor.apply_transformation.return_value = None
        mock_processor.write.return_value = None

        transformation_types = ['flip_x_axis', 'grayscale']
        input_dir = Path('./test_images').resolve()
        output_dir = Path('./output').resolve()

        transform_images(transformation_types, input_dir, output_dir)

        mock_processor.apply_transformation.assert_any_call('flip_x_axis')
        mock_processor.apply_transformation.assert_any_call('grayscale')
        mock_processor.write.assert_called()

    @patch('surescan_image_transformer_core_wrapper.glob.glob')
    def test_find_images(self, mock_glob):
        mock_glob.return_value = [str(Path('./test_images/image1.jpg').resolve()), str(Path('./test_images/image1.jpg').resolve())]
        images = find_images(Path('./test_images').resolve())
        self.assertEqual(images[0], Path('./test_images/image1.jpg').resolve())

    @patch('surescan_image_transformer_core_wrapper.Path.exists')
    @patch('surescan_image_transformer_core_wrapper.Path.mkdir')
    def test_transform_images_directory_creation(self, mock_mkdir, mock_exists):
        mock_exists.side_effect = [True, False]  # input_dir exists, output_dir does not

        transformation_types = ['rotate_left']
        input_dir = Path('./test_images').resolve()
        output_dir = Path('./output').resolve()

        with patch('surescan_image_transformer_core_wrapper.find_images', return_value=[Path('./test_images/image1.jpg').resolve()]):
            transform_images(transformation_types, input_dir, output_dir)

        mock_mkdir.assert_called_once()

if __name__ == '__main__':
    unittest.main()
