#include "mainwindow.h"
#include "imagewindow.h"
#include <QHBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(Client *client, QWidget *parent) :
    QMainWindow(parent),
    client(client)
{
    QWidget *centralWidget = new QWidget(this);

    label = new QLabel("Auth+", this);
    textLineEdit = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);
    browseButton = new QPushButton("Browse Image", this);

    RSAtext = new QLineEdit(this);
    RSAencrypt = new QLabel("Зашифрованный текст RSA: ", this);
    RSAButton = new QPushButton("Зашифровать", this);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(label);
    layout->addWidget(textLineEdit);
    layout->addWidget(browseButton);
    layout->addWidget(sendButton);
    layout->addWidget(RSAencrypt);
    layout->addWidget(RSAtext);
    layout->addWidget(RSAButton);

    setCentralWidget(centralWidget);

    connect(sendButton, &QPushButton::clicked, this, &MainWindow::on_sendButton_clicked);
    connect(browseButton, &QPushButton::clicked, this, &MainWindow::on_browseButton_clicked);
    connect(RSAButton, &QPushButton::clicked, this, &MainWindow::on_RSAButton_clicked);
    connect(client, &Client::imageReceived, this, &MainWindow::onImageReceived);
    connect(client, &Client::receivedResponse, this, &MainWindow::handleServerResponse);


    setWindowTitle("Main Application");
    resize(400, 300);
}

MainWindow::~MainWindow()
{
    delete label;
    delete textLineEdit;
    delete sendButton;
    delete browseButton;
}

void MainWindow::on_sendButton_clicked()
{
    QString text = textLineEdit->text();
    client->sendTextAndImage(text, selectedFilePath);
}

void MainWindow::on_browseButton_clicked()
{
    selectedFilePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!selectedFilePath.isEmpty()) {
        label->setText("Image selected: " + selectedFilePath);
    } else {
        label->setText("No image selected");
    }
}

void MainWindow::on_RSAButton_clicked(){
    QString text = RSAtext->text();
    client->sendRSAtext(text);
}

void MainWindow::onImageReceived(const QByteArray &imageData)
{
    qDebug() << "Image received in MainWindow. Size:" << imageData.size();
    ImageWindow *imageWindow = new ImageWindow(client);
    imageWindow->onImageReceived(imageData);
    imageWindow->show();
    this->hide();
}

void MainWindow::handleServerResponse(const QString &response)
{
    RSAencrypt->setText(response);
}
