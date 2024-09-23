#ifndef SINGL_CLIENT_H
#define SINGL_CLIENT_H

#include <QObject>
#include <QTcpSocket>

class singl_client : public QObject
{
    Q_OBJECT

public:
    static singl_client* instance();
    void sendRequest(const QString& request);

signals:
    void responseReceived(const QString& response);

private slots:
    void onReadyRead();

private:
    singl_client(QObject *parent = nullptr);
    static singl_client* m_instance;
    QTcpSocket m_socket;
};

#endif // SINGL_CLIENT_H

