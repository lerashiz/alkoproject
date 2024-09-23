#include <QApplication>
#include "mainwindow.h"
#include "loginwindow.h"
#include "authwindow.h"
#include "registerwindow.h"
#include "client.h"
#include "imagewindow.h"
#include "adminwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Client client;
    LoginWindow loginWindow(&client);
    AuthWindow authWindow(&client);
    RegisterWindow registerWindow(&client);
    AdminWindow adminWindow(&client);

    QObject::connect(&loginWindow, &LoginWindow::showAuthWindow, [&]() {
        loginWindow.hide();
        authWindow.show();
    });

    QObject::connect(&authWindow, &AuthWindow::showAdminWindow, [&](){
        authWindow.hide();
        AdminWindow *adminWindow = new AdminWindow(&client);
        adminWindow->show();
    });

    QObject::connect(&loginWindow, &LoginWindow::showRegisterWindow, [&]() {
        loginWindow.hide();
        registerWindow.show();
    });

    QObject::connect(&authWindow, &AuthWindow::backToLogin, [&]() {
        authWindow.hide();
        loginWindow.show();
    });

    QObject::connect(&registerWindow, &RegisterWindow::backToLogin, [&]() {
        registerWindow.hide();
        loginWindow.show();
    });

    QObject::connect(&authWindow, &AuthWindow::showMainWindow, [&]() {
        authWindow.hide();
        MainWindow *mainWindow = new MainWindow(&client);
        mainWindow->show();
    });

    QObject::connect(&client, &Client::imageReceived, [&]() {
        MainWindow *mainWindow = dynamic_cast<MainWindow *>(client.parent());
        if (mainWindow) {
            mainWindow->hide();
        }
        ImageWindow *imageWindow = new ImageWindow(&client);
        imageWindow->show();
    });

    loginWindow.show();
    return a.exec();
}
