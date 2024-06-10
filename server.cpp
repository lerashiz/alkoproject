#include "server.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDataStream>
#include "crypter.h"
Server::Server()
{
    DatabaseManager *dbManager = DatabaseManager::getInstance();
    QSqlDatabase db = dbManager->getDatabase();
    if (db.isOpen())
    {
        qDebug()<<"Listening and DB open";
    }
    else
    {
        qDebug()<< "DB not open";
    }
    if (this->listen(QHostAddress:: Any, 23232))
    {
        qDebug() << "sucksesfull";
    }
    else
    {
        qDebug() << "error";
    }
    nextBlockSize = 0;

}

void Server::SendToClient(QString str)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));

    if (Sockets.isEmpty()) {
        qDebug() << "No clients connected. Message not sent.";
        return;
    }

    for (QTcpSocket *clientSocket : Sockets) {
        qint64 bytesWritten = clientSocket->write(block);
        if (bytesWritten == -1) {
            qDebug() << "Error writing to socket:" << clientSocket->errorString();
        } else if (bytesWritten != block.size()) {
            qDebug() << "Failed to write the entire message to socket";
        } else {
            qDebug() << "Data sent successfully to client";
        }
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket,&QTcpSocket::readyRead,this,&Server::slotReadyRead);
    connect(socket,&QTcpSocket::disconnected,this,&Server::deleteLater);

    Sockets.push_back(socket);
    qDebug()<<"client connected"<<socketDescriptor;

}
void Server::processDatabased(const QString& queryStr) {
    QSqlQuery qry;
    if (qry.exec(queryStr)) {
        if (queryStr.startsWith("SELECT")) {
            if (qry.next()) {
                if (socket->state() == QAbstractSocket::ConnectedState) {
                    socket->write("Connection Successful");
                } else {
                    qDebug() << "Socket is not in ConnectedState";
                }
            } else {
                if (socket->state() == QAbstractSocket::ConnectedState) {
                    socket->write("Incorrect username or password");
                } else {
                    qDebug() << "Socket is not in ConnectedState";
                }
            }
        } else if (queryStr.startsWith("INSERT")) {
            if (socket->state() == QAbstractSocket::ConnectedState) {
                socket->write("Registration Successful");
            } else {
                qDebug() << "Socket is not in ConnectedState";
            }
        } else {
            qDebug() << "Unknown query type";
        }
    } else {
        qDebug() << "Error executing query:" << qry.lastError().text();
        if (socket->state() == QAbstractSocket::ConnectedState) {
            socket->write("Error executing query");
        } else {
            qDebug() << "Socket is not in ConnectedState";
        }
    }
}
void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    if (in.status() == QDataStream::Ok)
    {
        qDebug() << "read..";
        for (;;)
        {
            if (nextBlockSize == 0)
            {
                qDebug() << "nextBlockSize=0";
                if (socket->bytesAvailable() < 2)
                {
                    qDebug() << "Data<, break";
                    break;
                }
                in >> nextBlockSize;
                qDebug() << "NextBlockSize =" << nextBlockSize;
            }
            if (socket->bytesAvailable() < nextBlockSize)
            {
                qDebug() << "Data noot full, break";
                break;
            }
            QString str;
            QTime time;
            in >> time >> str;
            nextBlockSize = 0;
            qDebug() << str;
            QString decryptedStr = Crypter::decryptString(str);
            qDebug() << decryptedStr;

            if (decryptedStr.startsWith("ECHO "))
            {
                SendToClient(decryptedStr.mid(5));
            }
            else
            {
                processDatabased(decryptedStr);
            }
            break;
        }
    }
    else
    {
        qDebug() << "DataStream error";
    }
}