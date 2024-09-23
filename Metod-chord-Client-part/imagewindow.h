#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "client.h"

class ImageWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWindow(Client *client, QWidget *parent = nullptr);
    void onImageReceived(const QByteArray &imageData);  // Сделаем этот метод публичным

private:
    QLabel *imageLabel;
    QPushButton *saveButton;

    Client *client;
};

#endif // IMAGEWINDOW_H
