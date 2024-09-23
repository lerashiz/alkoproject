#include "mytcpserver.h"
#include "functionserver.h"
#include <QDebug>
#include <QCoreApplication>
#include<QString>

MyTcpServer::~MyTcpServer()
{

    mTcpServer->close();
    //server_status=0;
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        //server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection()
{
    QTcpSocket *newSocket = mTcpServer->nextPendingConnection();
    mTcpSockets.append(newSocket);

    newSocket->write("Hello, World!!! I am echo server!\r\n");

    connect(newSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(newSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
}


void MyTcpServer::slotServerRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

        QByteArray response = socket->readAll();
        // QString message = QString::fromUtf8(response).trimmed();
        // qDebug() << "Received message: " << message;
        socket->write(parsing(response));

}

void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket *disconnectedSocket = qobject_cast<QTcpSocket*>(sender());
    if (disconnectedSocket)
    {
        mTcpSockets.removeOne(disconnectedSocket);
        disconnectedSocket->deleteLater();
    }
}

