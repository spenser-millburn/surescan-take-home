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

class MainWindow : public QWidget {
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QVBoxLayout(this);
        btnOpen = new QPushButton("Open Image", this);
        btnProcess = new QPushButton("Process Image", this);
        infoLabel = new QLabel("Select an image to process", this);
        imageLabel = new QLabel(this); // Label to display the image
        imageLabel->setAlignment(Qt::AlignCenter);
        
        layout->addWidget(btnOpen);
        layout->addWidget(btnProcess);
        layout->addWidget(infoLabel);
        layout->addWidget(imageLabel); // Add the image label to the layout

        connect(btnOpen, &QPushButton::clicked, this, &MainWindow::openImage);
        connect(btnProcess, &QPushButton::clicked, this, &MainWindow::processImage);

        btnProcess->setEnabled(false);  // Disable process button until an image is loaded
    }

private:
    QLabel *infoLabel;
    QLabel *imageLabel;    // Pointer to the label for displaying images
    QPushButton *btnProcess;  // Pointer to the process button
    QPushButton *btnOpen;     // Pointer to the open button
    QString imagePath;

    void openImage() {
        imagePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.jpeg)");
        if (!imagePath.isEmpty()) {
            QPixmap pixmap(imagePath);
            imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            infoLabel->setText("Image loaded: " + imagePath);
            btnProcess->setEnabled(true);  // Enable the process button
        }
    }

    void processImage() {
        if (imagePath.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please load an image first.");
            return;
        }

        ImageProcessor imgProcessor(imagePath.toStdString());
        imgProcessor.flipped_grayscale("./output.jpg");
        
        QPixmap outputPixmap("./output.jpg");
        imageLabel->setPixmap(outputPixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QMessageBox::information(this, "Success", "Processing complete! Output displayed.");
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Image Processor GUI");
    window.setFixedSize(600, 400);  // Set a fixed size for the window
    window.show();
    return app.exec();
}
