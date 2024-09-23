#include "loginwindow.h"
#include <QLabel>

LoginWindow::LoginWindow(Client *client, QWidget *parent) :
    QWidget(parent),
    client(client)
{
    authButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *titleLabel = new QLabel("Welcome", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(titleLabel);
    layout->addWidget(authButton);
    layout->addWidget(registerButton);

    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);

    setLayout(layout);

    connect(authButton, &QPushButton::clicked, this, &LoginWindow::openAuthWindow);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::openRegisterWindow);

    setWindowTitle("Welcome");
    resize(300, 200);
}

void LoginWindow::openAuthWindow()
{
    emit showAuthWindow();
}

void LoginWindow::openRegisterWindow()
{
    emit showRegisterWindow();
}
