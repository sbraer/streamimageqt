#include "nltcpsocket.h"
#include <iostream>
NLTcpSocket :: NLTcpSocket (QTcpSocket* socket)
{
    tcpSocket = socket;
    connect (tcpSocket, SIGNAL (readyRead()), this, SLOT (slotReadyRead()));
    connect (tcpSocket, SIGNAL (disconnected()), this, SLOT (slotDisconnected()));
    connect (tcpSocket, SIGNAL (connected()), this, SLOT (slotConnected()));
}

NLTcpSocket :: ~NLTcpSocket ()
{
#ifdef QT_DEBUG
    std::cout << "delete NLTcpSocket\n";
#endif
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

#ifdef QT_DEBUG
    qDebug() << "From client: " << ba;
#endif

    return QString(ba);
}

