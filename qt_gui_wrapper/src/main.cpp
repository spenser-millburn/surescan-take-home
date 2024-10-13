#include "image_processor.h"
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <iostream>
#include <vector>

class ImageProcessorGUI : public QWidget {
public:
    ImageProcessorGUI(QWidget *parent = nullptr) : QWidget(parent), imgProcessor(nullptr) {
        auto *layout = new QVBoxLayout(this);

        btnOpen = new QPushButton("Open Image", this);
        btnSave = new QPushButton("Save Processed Image", this);
        infoLabel = new QLabel("Select an image to process", this);
        pixelDensityLabel= new QLabel("", this);
        imageLabel = new QLabel(this); // Label to display the image
        imageLabel->setAlignment(Qt::AlignCenter);
        
        layout->addWidget(btnOpen);
        layout->addWidget(infoLabel);
        layout->addWidget(pixelDensityLabel);
        layout->addWidget(imageLabel); // Add the image label to the layout

        connect(btnOpen, &QPushButton::clicked, this, &ImageProcessorGUI::openImage);
        connect(btnSave, &QPushButton::clicked, this, &ImageProcessorGUI::saveImage);

        layout->addWidget(btnSave);

        disableButtons();  // Initially disable transformation buttons until an image is loaded
    }

private:
    QLabel *infoLabel;
    QLabel *pixelDensityLabel;
    QLabel *imageLabel;    // Pointer to the label for displaying images
    QPushButton *btnSave;  // Button to save the processed image
    QPushButton *btnOpen;  // Button to open the image
    QString imagePath;

    ImageProcessor *imgProcessor;
    std::vector<QPushButton*> transformationBtnList;

    void openImage() {
        imagePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.jpeg)");

        if (!imagePath.isEmpty()) {
            imgProcessor = new ImageProcessor(imagePath.toStdString());
            QPixmap pixmap(imagePath);
            imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            infoLabel->setText("Image loaded: " + imagePath);
            pixelDensityLabel->setText(QString("Pixel Density %1").arg(imgProcessor->get_average_pixel_density()));




            // tried to make this extensible, create transformation buttons based on available transformations
            createTransformationButtons();

            enableButtons();  
        }
    }

void createTransformationButtons() 
{
    const auto& transformations = imgProcessor->m_transformations_map;
    QVBoxLayout *layout = static_cast<QVBoxLayout*>(this->layout());

    // Clear existing transformation buttons
    for (auto btn : transformationBtnList) 
    {
        layout->removeWidget(btn);
        btn->deleteLater();
    }
    transformationBtnList.clear();

    if (transformations.find("reset_image") != transformations.end()) 
    {
        QPushButton *resetBtn = new QPushButton("reset_image", this);
        layout->addWidget(resetBtn);
        connect(resetBtn, &QPushButton::clicked, [this]() 
        {
            applyTransformation("reset_image");
        });
        transformationBtnList.push_back(resetBtn);
    }

    for (const auto &transformation : transformations) 
    {
        if (transformation.first == "reset_image") 
        {
            continue; // Skip, since we already added it
        }

        QPushButton *btn = new QPushButton(QString::fromStdString(transformation.first), this);
        layout->addWidget(btn);
        connect(btn, &QPushButton::clicked, [this, transformation]() 
        {
            applyTransformation(transformation.first);
            pixelDensityLabel->setText(QString("Pixel Density %1").arg(imgProcessor->get_average_pixel_density()));
        });
        transformationBtnList.push_back(btn);
    }
}

    void applyTransformation(const std::string &transformation_name) 
    {
        if (imgProcessor) 
        {
            std::cout << "applying transformation: " << transformation_name << std::endl;
            imgProcessor->apply_transformation(transformation_name);  // Apply the transformation
            updateImageLabel();  // Update the displayed image
        }
    }

       void updateImageLabel() 
       {
        if (imgProcessor) 
        {
            imgProcessor->write_image("./temp_output.jpg", "jpg");
            QPixmap outputPixmap("./temp_output.jpg");
            imageLabel->setPixmap(outputPixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }

    void saveImage() 
    {
            if (imgProcessor) 
            {
                QString savePath = QFileDialog::getSaveFileName(this, "Save Processed Image", "", "Images (*.png *.jpg *.jpeg)");
                if (!savePath.isEmpty()) 
                {
                    imgProcessor->write_image(savePath.toStdString(), "jpg");
                    QMessageBox::information(this, "Success", "Image saved to: " + savePath);
                }
            }
        }


    void disableButtons() 
    {
        for (auto btn : transformationBtnList) 
        {
            btn->setEnabled(false);
        }
        btnSave->setEnabled(false);
    }

    void enableButtons() 
    {
        for (auto btn : transformationBtnList) 
        {
            btn->setEnabled(true);
        }
        btnSave->setEnabled(true);
    }
};

int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);
    ImageProcessorGUI window;
    window.setWindowTitle("Surescan ImageProcessor Library GUI");
    window.setFixedSize(600, 400);  
    window.show();
    return app.exec();
}
