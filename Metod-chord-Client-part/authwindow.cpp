#include "authwindow.h"

AuthWindow::AuthWindow(Client *client, QWidget *parent) :
    QWidget(parent),
    client(client)
{
    usernameLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    loginButton = new QPushButton("Login", this);
    statusLabel = new QLabel("Status: ", this);
    QPushButton *backButton = new QPushButton("Back", this);

    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Username:", this));
    layout->addWidget(usernameLineEdit);
    layout->addWidget(new QLabel("Password:", this));
    layout->addWidget(passwordLineEdit);
    layout->addWidget(loginButton);
    layout->addWidget(statusLabel);
    layout->addWidget(backButton);

    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &AuthWindow::on_loginButton_clicked);
    connect(client, &Client::receivedResponse, this, &AuthWindow::handleServerResponse);
    connect(client, &Client::authenticationSuccess, this, &AuthWindow::openMainWindow);
    connect(client, &Client::adminSuccess, this, &AuthWindow::openAdminWindow);
    connect(backButton, &QPushButton::clicked, this, &AuthWindow::backToLogin);

    setWindowTitle("Login");
    resize(300, 200);
}

void AuthWindow::on_loginButton_clicked()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    client->sendAuthentication(username, password);
}

void AuthWindow::handleServerResponse(const QString &response)
{
    statusLabel->setText(response);
}

void AuthWindow::openMainWindow()
{
    emit showMainWindow();
}

void AuthWindow::openAdminWindow(){
    emit showAdminWindow();
}
