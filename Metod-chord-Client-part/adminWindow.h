#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "client.h"

class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(Client *client, QWidget *parent = nullptr);


signals:

private slots:
    void DeleteButtonClicked();
    void sqlButtonClick();
    void getUsersButtonClick();
    void handleServerResponse(const QString &response);

private:
    QTableWidget *usersTableWidget;
    QPushButton *getUsersButton;
    QLabel *deleteLine;
    QLineEdit *deleteUserLine;
    QPushButton *deleteButton;
    QLabel *sqlLabel;
    QLineEdit *sqlQuery;
    QPushButton *sqlButton;
    QLabel *statusLabel;

    Client *client;

};


#endif // ADMINWINDOW_H
