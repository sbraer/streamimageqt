#include "nltcpsocket.h"

NLTcpSocket::NLTcpSocket (QTcpSocket* socket) : m_pQTcpSocket{socket}
{
    qDebug("Constructor NLTcpSocket");
    connect(m_pQTcpSocket, &QTcpSocket::readyRead, this, &NLTcpSocket::slotReadyRead);
    connect(m_pQTcpSocket, &QTcpSocket::disconnected, this, &NLTcpSocket::slotDisconnected);
    connect(m_pQTcpSocket, &QTcpSocket::connected, this, &NLTcpSocket::slotConnected);
}

NLTcpSocket::~NLTcpSocket ()
{
    qDebug("Deconstructor NLTcpSocket");
    m_pQTcpSocket->deleteLater();
}

void NLTcpSocket::slotReadyRead ()
{
    emit dataReady(this);
}

void NLTcpSocket::slotConnected()
{
    emit socketConnected(this);
}

void NLTcpSocket::slotDisconnected()
{
    emit socketDisconnected(this);
}

void NLTcpSocket::writeMessage(const QString &msg)
{
    m_pQTcpSocket->write(msg.toStdString().append("\r\n").c_str());
}

void NLTcpSocket::writeMessageBinary(const QByteArray& msg)
{
    m_pQTcpSocket->write(msg);
}

QString NLTcpSocket::getData()
{
    QByteArray ba = m_pQTcpSocket->readAll();
    qDebug("From client: %s", ba.toStdString().c_str());
    return QString(ba);
}
