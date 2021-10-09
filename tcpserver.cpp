#include "tcpserver.h"
#include "nltcpsocket.h"
#include <QProcess>
#include <QString>
#include <QMetaMethod>

#define PORT 7474

TcpServer::TcpServer(QObject *parent) : QObject{parent}, m_bClientConnected{false}
{
    qDebug("Constructor TcpServer");
    m_pQTcpServer = std::make_unique<QTcpServer>();
    connect(m_pQTcpServer.get(), &QTcpServer::newConnection, this, &TcpServer::slotNewConnection);

    if(!m_pQTcpServer->listen(QHostAddress::Any, PORT))
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
    qDebug("Deconstructor TcpServer");
}

void TcpServer::slotNewConnection()
{
    qInfo()<< "Connected";
    QTcpSocket* socket = m_pQTcpServer->nextPendingConnection();
    auto customSocket = new NLTcpSocket{socket};

    QString text = "HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace; boundary=--boundary\r\n";
    socket->write(text.toStdString().c_str());

    connect(this, &TcpServer::sendMessage, customSocket, &NLTcpSocket::writeMessage);
    connect(this, &TcpServer::sendMessageBinary, customSocket, &NLTcpSocket::writeMessageBinary);
    connect(customSocket, &NLTcpSocket::dataReady,this, &TcpServer::slotReceive);
    connect(customSocket, &NLTcpSocket::socketDisconnected,this, &TcpServer::slotDisconnectSocket);

    QMutexLocker<QMutex> ml{&m_pQMutex};
    if (m_bClientConnected == false) {
        m_bClientConnected = true;
        emit isClientConnected(m_bClientConnected);
    }

    m_pNLTcpSocket.append(customSocket);
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

    QMutexLocker<QMutex> ml{&m_pQMutex};
    if (m_pNLTcpSocket.contains(socket)) {
        qDebug("Removed from QList");
        m_pNLTcpSocket.removeOne(socket);
    }

    socket->deleteLater();
    bool clientConnected = this->isSignalConnected(QMetaMethod::fromSignal(&TcpServer::sendMessage));
    if (m_bClientConnected != clientConnected) {
        m_bClientConnected = clientConnected;
        emit isClientConnected(m_bClientConnected);
    }
}

void TcpServer::closeConnection()
{
    m_pQTcpServer->close();
}
