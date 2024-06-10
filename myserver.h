#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QJsonParseError>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QSqlQuery>

class myserver: public QTcpServer
{
    Q_OBJECT
public:
    myserver();
    ~myserver();
    QTcpSocket *socket;
    QByteArray Data;

    QByteArray itog;

    QJsonDocument doc;
    QJsonParseError docError;

    QSqlDatabase db;
public slots:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);
    void sockReady();
    void sockDisc();
};


#endif // MYSERVER_H