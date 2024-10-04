import unittest
from unittest.mock import patch, MagicMock
from pathlib import Path
import os
from  main import *
from image_transformer import transform_image

class TestImageTransformation(unittest.TestCase):

    @patch('glob.glob')
    def test_find_images(self, mock_glob):
        mock_glob.return_value = ['/path/to/image1.jpg', '/path/to/image2.png']
        images = find_images('/some/dir')
        self.assertEqual(len(images), 2)
        self.assertEqual(images[0], Path('/path/to/image1.jpg'))

    @patch('os.makedirs')
    @patch('os.path.isdir')
    @patch('glob.glob')
    @patch('image_transformer.transform_image')
    def test_transform_images(self, mock_transform_image, mock_glob, mock_isdir, mock_makedirs):
        mock_isdir.side_effect = lambda x: x == './images'
        mock_transform_image.return_value = None
        transform_images(transformation_type="flipped_bw" , input_dir='./images/', output_dir='./output/')
        #TODO, make test cases for bad input image input, no path provided, 

        # mock_makedirs.assert_called_once_with('/output/dir/images', exist_ok=True)
        # mock_transform_image.assert_called_once()

if __name__ == '__main__':
    unittest.main()
