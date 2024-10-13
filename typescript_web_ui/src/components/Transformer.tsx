import React, { useState, useEffect } from 'react';
import { Box, Button, Typography, TextField, MenuItem } from '@mui/material';
import axios from 'axios';
import logo from '../assets/logo.svg'; // Import the logo

/**
 * Represents the FileUpload component.
 */
const Transformer: React.FC = () => {
  const [selectedFile, setSelectedFile] = useState<File | null>(null);
  const [transformationType, setTransformationType] = useState<string>('');
  const [imageUrl, setImageUrl] = useState<string | null>(null);
  const [transformations, setTransformations] = useState<string[]>([]);
  const [loading, setLoading] = useState<boolean>(false);
  const [uploadType, setUploadType] = useState<string>('single'); // Track whether uploading single image or tarball

  // Use the correct API host and ensure the FastAPI endpoint matches your backend
  const API_HOST = process.env.REACT_APP_API_HOST || 'http://localhost:8000';

  // Fetches available transformations from the API
  const fetchTransformations = async () => {
    setLoading(true);
    try {
      const response = await axios.get(`${API_HOST}/transformations`);
      setTransformations(response.data); // Assume backend sends an array of strings
      if (response.data.length > 0) {
        setTransformationType(response.data[0]); // Set default to the first option
      }
    } catch (error) {
      console.error("Error fetching transformations:", error);
      alert('There was an error fetching the transformation types.');
    } finally {
      setLoading(false);
    }
  };

  // Fetch transformations when component mounts
  useEffect(() => {
    fetchTransformations();
  }, []);

  // Handle file selection
  const handleFileChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0] || null;
    setSelectedFile(file);
  };

  // Handle transformation type change
  const handleTransformationChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    setTransformationType(event.target.value);
  };

  // Handle upload type change (single image or tarball)
  const handleUploadTypeChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    console.log("Upload type selected:", event.target.value); // Debugging statement
    setUploadType(event.target.value);
  };

  // Handles file upload to FastAPI depending on the upload type
  const handleUpload = async () => {
    if (!selectedFile) {
      alert('Please select a file to upload.');
      return;
    }

    const formData = new FormData();
    formData.append('file', selectedFile);
    formData.append('transformation_types', transformationType); // Include the transformation type

    try {
      let response;
      if (uploadType === 'single') {
        // Upload to process_single_image endpoint
        response = await axios.post(`${API_HOST}/process_single_image`, formData, {
          headers: { 'Content-Type': 'multipart/form-data' },
          responseType: 'blob',
        });

        const blob = new Blob([response.data], { type: 'image/png' });
        const downloadUrl = window.URL.createObjectURL(blob);
        setImageUrl(downloadUrl);
      } else {
        // Upload to process_images endpoint (tarball processing)
        response = await axios.post(`${API_HOST}/process_images`, formData, {
          headers: { 'Content-Type': 'multipart/form-data' },
          responseType: 'blob',
        });

        const blob = new Blob([response.data], { type: 'application/x-tar' });
        const downloadUrl = window.URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = downloadUrl;
        a.download = 'transformed_images.tar.gz';
        a.click();
      }
    } catch (error) {
      console.error('Error uploading file:', error);
      alert('There was an error processing the file.');
    }
  };

  return (
    <Box sx={{ display: 'flex', flexDirection: 'column', marginTop: '20px' }}>
      {/* Logo at the top */}
      <Box sx={{ display: 'flex', justifyContent: 'center', marginBottom: '20px' }}>
        <img src={logo} alt="Logo" style={{ width: '1000px' }} />
      </Box>

      <Typography variant="h6" sx={{ marginBottom: '10px' }}>
        C++ Image Transformer Library Demo - Web Client
      </Typography>
      <input type="file" onChange={handleFileChange} style={{ marginBottom: '10px' }} />

      <TextField
        select
        label="Upload Type"
        value={uploadType}
        onChange={handleUploadTypeChange}
        sx={{ marginBottom: '10px' }}
      >
        <MenuItem value="single">Single Image</MenuItem>
        <MenuItem value="tarball">Tarball of Images</MenuItem>
      </TextField>

      <TextField
        select
        label="Transformation Type"
        value={transformationType}
        onChange={handleTransformationChange}
        sx={{ marginBottom: '10px' }}
        disabled={transformations.length === 0}
      >
        {transformations.map((transformation) => (
          <MenuItem key={transformation} value={transformation}>
            {transformation.replace(/_/g, ' ').replace(/\b\w/g, (char) => char.toUpperCase())} {/* Format display */}
          </MenuItem>
        ))}
      </TextField>

      <Button
        variant="contained"
        color="primary"
        onClick={handleUpload}
        disabled={!selectedFile}
      >
        Upload
      </Button>

      {uploadType === 'single' && imageUrl && (
        <Box sx={{ marginTop: '20px' }}>
          <Typography variant="h6">Transformed Image:</Typography>
          <img src={imageUrl} alt="Transformed File" style={{ maxWidth: '100%', maxHeight: '400px' }} />
        </Box>
      )}
    </Box>
  );
};

export default Transformer;
