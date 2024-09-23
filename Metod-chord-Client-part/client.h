#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    void connectToServer();
    void sendRegistration(const QString &username, const QString &password, const QString &email);
    void sendAuthentication(const QString &username, const QString &password);
    void sendTextAndImage(const QString &text, const QString &imagePath);
    void sendRSAtext(const QString &text);
    void deleteUser(const QString &username);
    void makeSqlQuery(const QString &query);
    void getUsers();

signals:
    void receivedResponse(const QString &response);
    void authenticationSuccess();
    void adminSuccess();
    void imageReceived(const QByteArray &imageData);

private slots:
    void onConnected();
    void onReadyRead();

private:
    QTcpSocket *mTcpSocket;
    QByteArray buffer;
};

#endif // CLIENT_H
