#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "client.h"

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(Client *client, QWidget *parent = nullptr);

signals:
    void showAuthWindow();
    void showRegisterWindow();

private slots:
    void openAuthWindow();
    void openRegisterWindow();

private:
    QPushButton *authButton;
    QPushButton *registerButton;

    Client *client;
};

#endif // LOGINWINDOW_H
