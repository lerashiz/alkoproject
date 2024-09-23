#include "adminWindow.h"

AdminWindow::AdminWindow(Client *client, QWidget *parent) :
    QWidget(parent),
    client(client)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    usersTableWidget = new QTableWidget(0,2, this);
    layout->addWidget(usersTableWidget);
    QStringList headers = {"Username", "Email"};
    usersTableWidget->setHorizontalHeaderLabels(headers);

    getUsersButton = new QPushButton("Получить всех пользователей", this);
    deleteUserLine = new QLineEdit(this);
    deleteButton = new QPushButton("Удалить пользователя", this);

    sqlLabel = new QLabel("Введите sql запрос", this);
    sqlQuery = new QLineEdit(this);
    sqlButton = new QPushButton("Отправить запрос", this);

    statusLabel = new QLabel("Статус:", this);

    layout->addWidget(getUsersButton);
    layout->addWidget(sqlLabel);
    layout->addWidget(sqlQuery);
    layout->addWidget(sqlButton);
    layout->addWidget(deleteUserLine);
    layout->addWidget(deleteButton);
    layout->addWidget(statusLabel);


    connect(getUsersButton, &QPushButton::clicked, this, &AdminWindow::getUsersButtonClick);
    connect(sqlButton, &QPushButton::clicked, this, &AdminWindow::sqlButtonClick);
    connect(client, &Client::receivedResponse, this, &AdminWindow::handleServerResponse);
    connect(deleteButton, &QPushButton::clicked, this, &AdminWindow::DeleteButtonClicked);

    setWindowTitle("Admin");
    resize(400, 400);
}


void AdminWindow::DeleteButtonClicked(){
    QString user = deleteUserLine->text();
    client->deleteUser(user);
}

void AdminWindow::getUsersButtonClick(){
    client->getUsers();
}

void AdminWindow::sqlButtonClick(){
    QString query = sqlQuery->text();
    client->makeSqlQuery(query);
}

void AdminWindow::handleServerResponse(const QString &response){
    QStringList clue_words = response.split(';');
    QString action = clue_words.front();
    clue_words.pop_front();
    if (action == "sql"){
        QStringList nicks = clue_words.at(0).split(',');
        QStringList emails = clue_words.at(1).split(',');
        for(int i = 0; i < nicks.length(); ++i){
            usersTableWidget->setRowCount(nicks.length());
            QTableWidgetItem *nick = new QTableWidgetItem(nicks.at(i));
            QTableWidgetItem *email = new QTableWidgetItem(emails.at(i));
            usersTableWidget->setItem(i, 0, nick);
            usersTableWidget->setItem(i, 1, email);
        }
    }
    else{
        statusLabel->setText("Status: " + response);
    }
    }
