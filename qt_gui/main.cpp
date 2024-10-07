#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QPixmap>
#include <QWidget>
#include <QImage>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget mainWindow;
    QVBoxLayout *layout = new QVBoxLayout(&mainWindow);

    QLabel *imageLabel = new QLabel(&mainWindow);
    QPushButton *openButton = new QPushButton("Open Image", &mainWindow);
    QPushButton *rotateButton = new QPushButton("Rotate", &mainWindow);
    QPushButton *flipButton = new QPushButton("Flip", &mainWindow);
    QPushButton *grayscaleButton = new QPushButton("Grayscale", &mainWindow);

    layout->addWidget(imageLabel);
    layout->addWidget(openButton);
    layout->addWidget(rotateButton);
    layout->addWidget(flipButton);
    layout->addWidget(grayscaleButton);

    QImage image;

    QObject::connect(openButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&mainWindow, "Open Image File", "", "Images (*.png *.xpm *.jpg)");
        if (!fileName.isEmpty()) {
            image.load(fileName);
            imageLabel->setPixmap(QPixmap::fromImage(image));
        }
    });

    QObject::connect(rotateButton, &QPushButton::clicked, [&]() {
        if (!image.isNull()) {
            QMatrix rm;
            rm.rotate(90);
            image = image.transformed(rm);
            imageLabel->setPixmap(QPixmap::fromImage(image));
        }
    });

    QObject::connect(flipButton, &QPushButton::clicked, [&]() {
        if (!image.isNull()) {
            image = image.mirrored(true, false);
            imageLabel->setPixmap(QPixmap::fromImage(image));
        }
    });

    QObject::connect(grayscaleButton, &QPushButton::clicked, [&]() {
        if (!image.isNull()) {
            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    QColor color = image.pixelColor(x, y);
                    int gray = qGray(color.rgb());
                    image.setPixelColor(x, y, QColor(gray, gray, gray));
                }
            }
            imageLabel->setPixmap(QPixmap::fromImage(image));
        }
    });

    mainWindow.setLayout(layout);
    mainWindow.setWindowTitle("MyImageBrowser");
    mainWindow.show();

    return app.exec();
}
