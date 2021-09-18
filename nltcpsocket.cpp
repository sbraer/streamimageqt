#include "nltcpsocket.h"

NLTcpSocket :: NLTcpSocket (QTcpSocket* socket) : tcpSocket(socket)
{
    connect (tcpSocket, SIGNAL (readyRead()), this, SLOT (slotReadyRead()));
    connect (tcpSocket, SIGNAL (disconnected()), this, SLOT (slotDisconnected()));
    connect (tcpSocket, SIGNAL (connected()), this, SLOT (slotConnected()));
}

NLTcpSocket :: ~NLTcpSocket ()
{
    qDebug("delete NLTcpSocket");
    tcpSocket->deleteLater();
}

void NLTcpSocket :: slotReadyRead ()
{
    emit dataReady (this);
}

void NLTcpSocket :: slotConnected()
{
    emit socketConnected(this);
}

void NLTcpSocket :: slotDisconnected()
{
    emit socketDisconnected(this);
}

void NLTcpSocket::writeMessage(const QString &msg)
{
    tcpSocket->write(msg.toStdString().append("\r\n").c_str());
}

void NLTcpSocket::writeMessageBinary(const QByteArray& msg)
{
    tcpSocket->write(msg);
}

QString NLTcpSocket::getData()
{
    QByteArray ba = tcpSocket->readAll();
    qDebug("From client: %s", ba.toStdString().c_str());
    return QString(ba);
}

