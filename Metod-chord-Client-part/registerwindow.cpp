#include "registerwindow.h"

RegisterWindow::RegisterWindow(Client *client, QWidget *parent) :
    QWidget(parent),
    client(client)
{
    usernameLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    emailLineEdit = new QLineEdit(this);
    registerButton = new QPushButton("Register", this);
    statusLabel = new QLabel("Status: ", this);
    QPushButton *backButton = new QPushButton("Back", this);

    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Username:", this));
    layout->addWidget(usernameLineEdit);
    layout->addWidget(new QLabel("Password:", this));
    layout->addWidget(passwordLineEdit);
    layout->addWidget(new QLabel("Email:", this));
    layout->addWidget(emailLineEdit);
    layout->addWidget(registerButton);
    layout->addWidget(statusLabel);
    layout->addWidget(backButton);

    setLayout(layout);

    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::on_registerButton_clicked);
    connect(client, &Client::receivedResponse, this, &RegisterWindow::handleServerResponse);
    connect(backButton, &QPushButton::clicked, this, &RegisterWindow::backToLogin);

    setWindowTitle("Register");
    resize(300, 250);
}

void RegisterWindow::on_registerButton_clicked()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    QString email = emailLineEdit->text();
    client->sendRegistration(username, password, email);
}

void RegisterWindow::handleServerResponse(const QString &response)
{
    statusLabel->setText(response);
}
