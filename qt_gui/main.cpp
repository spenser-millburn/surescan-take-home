#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QPixmap>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget mainWindow;
    QVBoxLayout *layout = new QVBoxLayout(&mainWindow);

    QLabel *imageLabel = new QLabel(&mainWindow);
    QPushButton *openButton = new QPushButton("Open Image", &mainWindow);

    layout->addWidget(imageLabel);
    layout->addWidget(openButton);

    QObject::connect(openButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&mainWindow, "Open Image File", "", "Images (*.png *.xpm *.jpg)");
        if (!fileName.isEmpty()) {
            QPixmap image(fileName);
            imageLabel->setPixmap(image);
        }
    });

    mainWindow.setLayout(layout);
    mainWindow.setWindowTitle("MyImageBrowser");
    mainWindow.show();

    return app.exec();
}
