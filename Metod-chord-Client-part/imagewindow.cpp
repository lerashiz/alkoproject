#include "imagewindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

ImageWindow::ImageWindow(Client *client, QWidget *parent) :
    QWidget(parent),
    client(client)
{
    imageLabel = new QLabel(this);
    saveButton = new QPushButton("Save Image", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(imageLabel);
    layout->addWidget(saveButton);

    setLayout(layout);

    connect(saveButton, &QPushButton::clicked, [this]() {
        QString savePath = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *.jpeg)");
        if (!savePath.isEmpty()) {
            QPixmap pixmap = imageLabel->pixmap(Qt::ReturnByValue);
            pixmap.save(savePath);
        }
    });

    setWindowTitle("Image Viewer");
    resize(400, 300);
}

void ImageWindow::onImageReceived(const QByteArray &imageData)
{
    qDebug() << "Displaying received image. Size:" << imageData.size();
    QPixmap pixmap;
    pixmap.loadFromData(imageData);
    imageLabel->setPixmap(pixmap);
}
