#include "tcpserver.h"
#include "nltcpsocket.h"
#include <QProcess>
#include <QString>
#include <QMetaMethod>

#define PORT 7474

TcpServer::TcpServer(QObject *parent) : QObject(parent), _clientConnected(false)
{
    server = std::make_unique<QTcpServer>();
    connect(server.get(), &QTcpServer::newConnection, this, &TcpServer::slotNewConnection);

    if(!server->listen(QHostAddress::Any, PORT))
    {
        qCritical()<< "Server Could Not be Started";
    }
    else
    {
        qInfo()<< "Server Started";
    }
}

TcpServer::~TcpServer()
{
    qInfo()<< "Server Deleted";
}

void TcpServer::slotNewConnection()
{
    qInfo()<<"Connected";
    QTcpSocket* socket = server->nextPendingConnection();
    NLTcpSocket* customSocket = new NLTcpSocket(socket);

    QString text = "HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace; boundary=--boundary\r\n";
    socket->write(text.toStdString().c_str());

    connect(this, &TcpServer::sendMessage, customSocket, &NLTcpSocket::writeMessage);
    connect(this, &TcpServer::sendMessageBinary, customSocket, &NLTcpSocket::writeMessageBinary);
    connect(customSocket, &NLTcpSocket::dataReady,this, &TcpServer::slotReceive);
    connect(customSocket, &NLTcpSocket::socketDisconnected,this, &TcpServer::slotDisconnectSocket);

    QMutexLocker<QMutex> ml(&mMutex);
    if (_clientConnected == false) {
        _clientConnected = true;
        emit isClientConnected(_clientConnected);
    }

    sockets.append(customSocket);
}

void TcpServer::slotReceive(NLTcpSocket* socket)
{
    socket->getData();
}

void TcpServer::slotDisconnectSocket(NLTcpSocket* socket)
{
    qInfo()<< "Disconnected";
    disconnect(this, &TcpServer::sendMessage, socket, nullptr);
    disconnect(this, &TcpServer::sendMessageBinary, socket, nullptr);

    QMutexLocker<QMutex> ml(&mMutex);
    if (sockets.contains(socket)) {
        qDebug("Removed from QList");
        sockets.removeOne(socket);
    }

    socket->deleteLater();
    bool clientConnected = this->isSignalConnected(QMetaMethod::fromSignal(&TcpServer::sendMessage));
    if (_clientConnected != clientConnected) {
        _clientConnected = clientConnected;
        emit isClientConnected(_clientConnected);
    }
}

void TcpServer::closeConnection()
{
    server->close();
}
