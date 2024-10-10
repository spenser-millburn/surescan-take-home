#include "surescan_gui.h"
#include <QFileDialog>
#include <QPixmap>
#include <QColor>
#include <QApplication>
#include <Eigen/Dense>

using namespace Eigen;

ImageBrowser::ImageBrowser() {
    mainWindow = new QWidget;
    layout = new QVBoxLayout(mainWindow);

    imageLabel = new QLabel(mainWindow);
    imageLabel->setAlignment(Qt::AlignCenter);

    openButton = new QPushButton("Open Image", mainWindow);
    rotateButton = new QPushButton("Rotate", mainWindow);
    flipXButton = new QPushButton("Flip X-Axis", mainWindow);
    flipYButton = new QPushButton("Flip Y-Axis", mainWindow);
    grayscaleButton = new QPushButton("Grayscale", mainWindow);

    layout->addWidget(imageLabel);
    layout->addWidget(openButton);
    layout->addWidget(rotateButton);
    layout->addWidget(flipXButton);
    layout->addWidget(flipYButton);
    layout->addWidget(grayscaleButton);

    connectButtons();

    mainWindow->setLayout(layout);
    mainWindow->setWindowTitle("MyImageBrowser");
    mainWindow->setFixedSize(800, 600);
}

void ImageBrowser::show() {
    mainWindow->show();
}

void ImageBrowser::connectButtons() {
    QObject::connect(openButton, &QPushButton::clicked, [this]() {
        QString fileName = QFileDialog::getOpenFileName(mainWindow, "Open Image File", "", "Images (*.png *.xpm *.jpg)");
        if (!fileName.isEmpty()) {
            image.load(fileName);
            updateImageDisplay();
        }
    });

    QObject::connect(rotateButton, &QPushButton::clicked, [this]() {
        if (!image.isNull()) {
            Matrix3f rotationMatrix;
            float angle = 90.0f * M_PI / 180.0f; // Convert to radians
            rotationMatrix << cos(angle), -sin(angle), 0,
                              sin(angle),  cos(angle), 0,
                              0,           0,          1;

            applyEigenTransformation(rotationMatrix);
            updateImageDisplay();
        }
    });

    QObject::connect(flipXButton, &QPushButton::clicked, [this]() {
        if (!image.isNull()) {
            Matrix3f flipXMatrix;
            flipXMatrix << -1, 0, image.width(),
                           0, 1, 0,
                           0, 0, 1;

            applyEigenTransformation(flipXMatrix);
            updateImageDisplay();
        }
    });

    QObject::connect(flipYButton, &QPushButton::clicked, [this]() {
        if (!image.isNull()) {
            Matrix3f flipYMatrix;
            flipYMatrix << 1,  0, 0,
                           0, -1, image.height(),
                           0,  0, 1;

            applyEigenTransformation(flipYMatrix);
            updateImageDisplay();
        }
    });

    QObject::connect(grayscaleButton, &QPushButton::clicked, [this]() {
        if (!image.isNull()) {
            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    QColor color = image.pixelColor(x, y);
                    int gray = qGray(color.rgb());
                    image.setPixelColor(x, y, QColor(gray, gray, gray));
                }
            }
            updateImageDisplay();
        }
    });
}

void ImageBrowser::applyEigenTransformation(const Matrix3f &transformationMatrix) {
    QImage transformedImage(image.size(), image.format());
    transformedImage.fill(Qt::transparent);

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            Vector3f originalPoint(x, y, 1);
            Vector3f transformedPoint = transformationMatrix * originalPoint;
            int newX = static_cast<int>(transformedPoint.x());
            int newY = static_cast<int>(transformedPoint.y());

            if (newX >= 0 && newX < image.width() && newY >= 0 && newY < image.height()) {
                transformedImage.setPixelColor(newX, newY, image.pixelColor(x, y));
            }
        }
    }

    image = transformedImage;
}

void ImageBrowser::updateImageDisplay() {
    int fixedWidth = 400;
    int fixedHeight = 300;
    image = image.scaled(fixedWidth, fixedHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(QPixmap::fromImage(image));
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ImageBrowser browser;
    browser.show();
    return app.exec();
}
