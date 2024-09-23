#include "singl_client.h"

singl_client* singl_client::m_instance = nullptr;

singl_client::singl_client(QObject *parent) : QObject(parent)
{
    connect(&m_socket, &QTcpSocket::readyRead, this, &singl_client::onReadyRead);
    m_socket.connectToHost("127.0.0.1", 33333); // Change to your server address and port
}

singl_client* singl_client::instance()
{
    if (!m_instance)
        m_instance = new singl_client();
    return m_instance;
}

void singl_client::sendRequest(const QString& request)
{
    if (m_socket.state() == QAbstractSocket::ConnectedState)
        m_socket.write(request.toUtf8());
}

void singl_client::onReadyRead()
{
    QString response = QString::fromUtf8(m_socket.readAll());
    emit responseReceived(response);
}















