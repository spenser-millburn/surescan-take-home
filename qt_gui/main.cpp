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
    imageLabel->setAlignment(Qt::AlignCenter); // Center the image in the label
    QPushButton *openButton = new QPushButton("Open Image", &mainWindow);
    QPushButton *rotateButton = new QPushButton("Rotate", &mainWindow);
    QPushButton *flipXButton = new QPushButton("Flip X-Axis", &mainWindow);
    QPushButton *flipYButton = new QPushButton("Flip Y-Axis", &mainWindow);
    QPushButton *grayscaleButton = new QPushButton("Grayscale", &mainWindow);

    layout->addWidget(imageLabel);
    layout->addWidget(openButton);
    layout->addWidget(rotateButton);
    layout->addWidget(flipXButton);
    layout->addWidget(flipYButton);
    layout->addWidget(grayscaleButton);

    QImage image;

    QObject::connect(openButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&mainWindow, "Open Image File", "", "Images (*.png *.xpm *.jpg)");
        if (!fileName.isEmpty()) {
            image.load(fileName);
            int fixedWidth = 400; // Fixed image width
            int fixedHeight = 300; // Fixed image height
            image = image.scaled(fixedWidth, fixedHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imageLabel->setPixmap(QPixmap::fromImage(image));
        }
    });

    QObject::connect(rotateButton, &QPushButton::clicked, [&]() {
        if (!image.isNull()) {
            QMatrix rm;
            rm.rotate(90);
            image = image.transformed(rm);
            int fixedWidth = 400;
            int fixedHeight = 300;
            image = image.scaled(fixedWidth, fixedHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imageLabel->setPixmap(QPixmap::fromImage(image));
        }
    });

    QObject::connect(flipXButton, &QPushButton::clicked, [&]() {
        if (!image.isNull()) {
            image = image.mirrored(true, false); // Flip along the X-axis
            int fixedWidth = 400;
            int fixedHeight = 300;
            image = image.scaled(fixedWidth, fixedHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imageLabel->setPixmap(QPixmap::fromImage(image));
        }
    });

    QObject::connect(flipYButton, &QPushButton::clicked, [&]() {
        if (!image.isNull()) {
            image = image.mirrored(false, true); // Flip along the Y-axis
            int fixedWidth = 400;
            int fixedHeight = 300;
            image = image.scaled(fixedWidth, fixedHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
            int fixedWidth = 400;
            int fixedHeight = 300;
            image = image.scaled(fixedWidth, fixedHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imageLabel->setPixmap(QPixmap::fromImage(image));
        }
    });

    mainWindow.setLayout(layout);
    mainWindow.setWindowTitle("MyImageBrowser");
    mainWindow.setFixedSize(800, 600); // Set fixed window size
    mainWindow.show();

    return app.exec();
}
