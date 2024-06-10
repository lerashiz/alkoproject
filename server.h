#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlDatabase>



class Server: public QTcpServer
{
    Q_OBJECT
public:
    Server();
    QTcpSocket * socket;
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    void SendToClient(QString str);
    quint16 nextBlockSize;
    void processDatabased(const QString& queryStr);
    QSqlDatabase db;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
private:
    QString username;
};

#endif // SERVER_H