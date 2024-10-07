#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QPixmap>
#include <QWidget>
#include <QImage>

class ImageBrowser {
public:
    ImageBrowser() {
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

    void show() {
        mainWindow->show();
    }

private:
    QWidget *mainWindow;
    QVBoxLayout *layout;
    QLabel *imageLabel;
    QPushButton *openButton;
    QPushButton *rotateButton;
    QPushButton *flipXButton;
    QPushButton *flipYButton;
    QPushButton *grayscaleButton;
    QImage image;

    void connectButtons() {
        QObject::connect(openButton, &QPushButton::clicked, [this]() {
            QString fileName = QFileDialog::getOpenFileName(mainWindow, "Open Image File", "", "Images (*.png *.xpm *.jpg)");
            if (!fileName.isEmpty()) {
                image.load(fileName);
                updateImageDisplay();
            }
        });

        QObject::connect(rotateButton, &QPushButton::clicked, [this]() {
            if (!image.isNull()) {
                QMatrix rm;
                rm.rotate(90);
                image = image.transformed(rm);
                updateImageDisplay();
            }
        });

        QObject::connect(flipXButton, &QPushButton::clicked, [this]() {
            if (!image.isNull()) {
                image = image.mirrored(true, false);
                updateImageDisplay();
            }
        });

        QObject::connect(flipYButton, &QPushButton::clicked, [this]() {
            if (!image.isNull()) {
                image = image.mirrored(false, true);
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

    void updateImageDisplay() {
        int fixedWidth = 400;
        int fixedHeight = 300;
        image = image.scaled(fixedWidth, fixedHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(QPixmap::fromImage(image));
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ImageBrowser browser;
    browser.show();
    return app.exec();
}
