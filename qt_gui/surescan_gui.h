#ifndef SURESCAN_GUI_H
#define SURESCAN_GUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QString>
#include <Eigen/Dense>

class ImageBrowser {
public:
    ImageBrowser();
    void show();

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

    void connectButtons();
    void applyTransformation(const Eigen::Matrix3f &transformationMatrix);
    void updateImageDisplay();
};

#endif // SURESCAN_GUI_H
