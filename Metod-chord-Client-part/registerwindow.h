#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "client.h"

class RegisterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWindow(Client *client, QWidget *parent = nullptr);

signals:
    void backToLogin();

private slots:
    void on_registerButton_clicked();
    void handleServerResponse(const QString &response);

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *emailLineEdit;
    QPushButton *registerButton;
    QLabel *statusLabel;

    Client *client;
};

#endif // REGISTERWINDOW_H
