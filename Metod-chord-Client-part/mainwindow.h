#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QProgressDialog>
#include <QMovie>
#include "client.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Client *client, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();
    void on_browseButton_clicked();
    void onImageReceived(const QByteArray &imageData);
    void on_RSAButton_clicked();
    void handleServerResponse(const QString &response);

private:
    QLabel *label;
    QLineEdit *textLineEdit;
    QPushButton *sendButton;
    QPushButton *browseButton;
    QString selectedFilePath;
    QLineEdit *RSAtext;
    QLabel *RSAencrypt;
    QPushButton *RSAButton;

    Client *client;
};

#endif // MAINWINDOW_H
