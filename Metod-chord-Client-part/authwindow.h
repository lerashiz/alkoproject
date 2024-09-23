#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "client.h"

class AuthWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AuthWindow(Client *client, QWidget *parent = nullptr);

signals:
    void backToLogin();
    void showMainWindow();
    void showAdminWindow();

private slots:
    void on_loginButton_clicked();
    void handleServerResponse(const QString &response);
    void openMainWindow();
    void openAdminWindow();

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QLabel *statusLabel;

    Client *client;
};

#endif // AUTHWINDOW_H
