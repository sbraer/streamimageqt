#include "tcpserver.h"
#include "nltcpsocket.h"
#include <QProcess>
#include <QString>
#include <QMetaMethod>

#define PORT 7474

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    _clientConnected = false;
    server = new QTcpServer();
    connect(server, &QTcpServer::newConnection, this, &TcpServer::slotNewConnection);

    if(!server->listen(QHostAddress::Any, PORT))
    {
        qDebug()<< "Server Could Not be Started";
        return;
    }
    else
    {
        qDebug()<< "Server Started";
    }
}

TcpServer::~TcpServer()
{
    qDebug()<< "Server Deleted";
    delete server;
}

void TcpServer::slotNewConnection()
{
    qDebug()<<"Connected";
    QTcpSocket* socket = server->nextPendingConnection();
    NLTcpSocket* customSocket = new NLTcpSocket(socket);

    QString text = "HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace; boundary=--boundary\r\n";
    socket->write(text.toStdString().c_str());

    QObject::connect(this, &TcpServer::sendMessage, customSocket, &NLTcpSocket::writeMessage);
    QObject::connect(this, &TcpServer::sendMessageBinary, customSocket, &NLTcpSocket::writeMessageBinary);
    connect(customSocket, &NLTcpSocket::dataReady,this, &TcpServer::slotReceive);
    connect(customSocket, &NLTcpSocket::socketDisconnected,this, &TcpServer::slotDisconnectSocket);

    QMutexLocker ml(&mMutex);
    _clientConnected = true;
    emit isClientConnected(_clientConnected);
    sockets.append(customSocket);
}

void TcpServer::slotReceive(NLTcpSocket* socket)
{
    socket->getData();
}

void TcpServer::slotDisconnectSocket(NLTcpSocket* socket)
{
#ifdef QT_DEBUG
    qDebug()<< "Disconnected " << this->isSignalConnected(QMetaMethod::fromSignal(&TcpServer::sendMessage));
#endif

    QObject::disconnect(this, &TcpServer::sendMessage, socket, nullptr);
    QObject::disconnect(this, &TcpServer::sendMessageBinary, socket, nullptr);

    QMutexLocker ml(&mMutex);
    if (sockets.contains(socket)) {
#ifdef QT_DEBUG
        qDebug()<< "Removed from QList";
#endif
        sockets.removeOne(socket);
    }

    delete socket;
    _clientConnected = this->isSignalConnected(QMetaMethod::fromSignal(&TcpServer::sendMessage));
    emit isClientConnected(_clientConnected);
}

void TcpServer::closeConnection()
{
    server->close();
}
