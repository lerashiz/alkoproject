#include "myserver.h"
#include <QDebug>
myserver::myserver() {}

myserver::~myserver() {}

void myserver::startServer()
{
    if (this->listen(QHostAddress:: Any, 55555))
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:\\workers.db");
        if (db.open())
        {
            qDebug()<<"Listening and DB open";
        }
        else
            {
                qDebug()<< "DB not open";
            }
    }
    else
    {
        qDebug()<<"Not listening";
    }
}
void myserver::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket,SIGNAL (readyRead()),this, SLOT(sockReady()));
    connect(socket,SIGNAL(disconnected()),this, SLOT(sockDisc()));
    qDebug()<<socketDescriptor<<" Client connected";
    qDebug()<<"Send client connect status - YES";
    socket->write(QByteArray("{\"type\":\"connect\",\"status\":\"yes\"}"));
}

void myserver::sockReady()
{
    Data = socket->readAll();
    qDebug()<<"Data"<< Data;

    doc = QJsonDocument::fromJson(Data, &docError);


    //{"type":"select","params":"workers"}
    if (docError.errorString()=="no error occurred")
    {
        if ((doc.object().value("type").toString() == "recSize") && (doc.object().value("resp").toString() == "workers"))
        {
            // Создаем JSON объект для хранения результатов запроса
            QJsonObject resultObject;

            // Выполняем запрос к базе данных для получения списка работников
            QSqlQuery query(db);
            if (query.exec("SELECT name FROM listworkers"))
            {
                // Если запрос выполнен успешно, добавляем имена работников в JSON объект
                QJsonArray resultList;
                while (query.next())
                {
                    resultList.append(query.value(0).toString());
                }
                // Добавляем массив имен работников в JSON объект
                resultObject.insert("result", resultList);
            }
            else
            {
                // Если запрос не выполнен успешно, добавляем сообщение об ошибке в JSON объект
                resultObject.insert("error", "Query not success");
            }

            // Формируем JSON строку из объекта с результатами запроса
            QJsonDocument jsonResponse(resultObject);
            QByteArray responseBytes = jsonResponse.toJson();

            // Отправляем ответ клиенту с размером и содержимым ответа
            QString response = QString("{\"type\":\"size\",\"resp\":\"workers\",\"length\":%1}").arg(responseBytes.size());
            socket->write(response.toUtf8() + responseBytes);
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "select") && (doc.object().value("params").toString() == "workers"))
        {
            // Если пришел запрос типа "select" с параметром "workers", отправляем сохраненный ранее результат запроса
            socket->write(itog);
            qDebug() << "Razmer soobjenia: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
    }
}
void myserver::sockDisc()
{
    qDebug()<<"Disconect";
    socket->deleteLater();
}