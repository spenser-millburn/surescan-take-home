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
    ImageProcessorGUI(QWidget *parent = nullptr) : QWidget(parent), m_img_processor(nullptr) {
        auto *l_layout = new QVBoxLayout(this);

        m_btn_open = new QPushButton("Open Image", this);
        m_btn_save = new QPushButton("Save Processed Image", this);
        m_info_label = new QLabel("Select an image to process", this);
        m_pixel_density_label = new QLabel("", this);
        m_image_label = new QLabel(this); // Label to display the image
        m_image_label->setAlignment(Qt::AlignCenter);

        l_layout->addWidget(m_btn_open);
        l_layout->addWidget(m_info_label);
        l_layout->addWidget(m_pixel_density_label);
        l_layout->addWidget(m_image_label); // Add the image label to the layout

        connect(m_btn_open, &QPushButton::clicked, this, &ImageProcessorGUI::openImage);
        connect(m_btn_save, &QPushButton::clicked, this, &ImageProcessorGUI::saveImage);

        l_layout->addWidget(m_btn_save);

        disableButtons();  // Initially disable transformation buttons until an image is loaded
    }

private:
    QLabel *m_info_label;
    QLabel *m_pixel_density_label;
    QLabel *m_image_label;    // Pointer to the label for displaying images
    QPushButton *m_btn_save;  // Button to save the processed image
    QPushButton *m_btn_open;  // Button to open the image
    QString m_image_path;
    ImageProcessor *m_img_processor;
    std::vector<QPushButton*> m_transformation_btn_list;

    void openImage() {
        m_image_path = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.jpeg)");

        if (!m_image_path.isEmpty()) {
            m_img_processor = new ImageProcessor(m_image_path.toStdString());
            QPixmap l_pixmap(m_image_path);
            m_image_label->setPixmap(l_pixmap.scaled(m_image_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            m_info_label->setText("Image loaded: " + m_image_path);
            m_pixel_density_label->setText(QString("Pixel Density %1").arg(m_img_processor->get_average_pixel_density()));
            // tried to make this extensible, create transformation buttons based on available transformations
            createTransformationButtons();

            enableButtons();
        }
    }

    void createTransformationButtons() {
        const auto& l_transformations = m_img_processor->m_transformations_map;
        QVBoxLayout *l_layout = static_cast<QVBoxLayout*>(this->layout());

        // Clear existing transformation buttons
        for (auto l_btn : m_transformation_btn_list) {
            l_layout->removeWidget(l_btn);
            l_btn->deleteLater();
        }
        m_transformation_btn_list.clear();

        if (l_transformations.find("reset_image") != l_transformations.end()) {
            QPushButton *l_reset_btn = new QPushButton("reset_image", this);
            l_layout->addWidget(l_reset_btn);
            connect(l_reset_btn, &QPushButton::clicked, [this]() {
                applyTransformation("reset_image");
            });
            m_transformation_btn_list.push_back(l_reset_btn);
        }

        for (const auto &l_transformation : l_transformations) {
            if (l_transformation.first == "reset_image") {
                continue; // Skip, since we already added it
            }

            QPushButton *l_btn = new QPushButton(QString::fromStdString(l_transformation.first), this);
            l_layout->addWidget(l_btn);
            connect(l_btn, &QPushButton::clicked, [this, l_transformation]() {
                applyTransformation(l_transformation.first);
                m_pixel_density_label->setText(QString("Pixel Density %1").arg(m_img_processor->get_average_pixel_density()));
            });
            m_transformation_btn_list.push_back(l_btn);
        }
    }

    void applyTransformation(const std::string &l_transformation_name) {
        if (m_img_processor) {
            std::cout << "applying transformation: " << l_transformation_name << std::endl;
            m_img_processor->apply_transformation(l_transformation_name);  // Apply the transformation
            updateImageLabel();  // Update the displayed image
        }
    }

    void updateImageLabel() {
        if (m_img_processor) {
            m_img_processor->write_image("./temp_output.jpg", "jpg");
            QPixmap l_output_pixmap("./temp_output.jpg");
            m_image_label->setPixmap(l_output_pixmap.scaled(m_image_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }

    void saveImage() {
        if (m_img_processor) {
            QString l_save_path = QFileDialog::getSaveFileName(this, "Save Processed Image", "", "Images (*.png *.jpg *.jpeg)");
            if (!l_save_path.isEmpty()) {
                m_img_processor->write_image(l_save_path.toStdString(), "jpg");
                QMessageBox::information(this, "Success", "Image saved to: " + l_save_path);
            }
        }
    }

    void disableButtons() {
        for (auto l_btn : m_transformation_btn_list) {
            l_btn->setEnabled(false);
        }
        m_btn_save->setEnabled(false);
    }

    void enableButtons() {
        for (auto l_btn : m_transformation_btn_list) {
            l_btn->setEnabled(true);
        }
        m_btn_save->setEnabled(true);
    }
};

int main(int argc, char *argv[]) {
    QApplication l_app(argc, argv);
    ImageProcessorGUI l_window;
    l_window.setWindowTitle("Surescan ImageProcessor Library GUI");
    l_window.setFixedSize(600, 400);

    l_window.show();
    return l_app.exec();
}
