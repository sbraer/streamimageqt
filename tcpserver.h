#ifndef NATIVE_LAUNCHER_H
#define NATIVE_LAUNCHER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include <QMutex>
#include <QMutexLocker>

class NLTcpSocket;

class TcpServer final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(TcpServer)

private:
    std::unique_ptr<QTcpServer> server;
    QList<NLTcpSocket*> sockets;
    bool _clientConnected;
    QMutex mMutex;

public:
    TcpServer(QObject *parent=nullptr);
    ~TcpServer() override;

public slots:
    void slotNewConnection();
    void slotReceive(NLTcpSocket* socket);
    void slotDisconnectSocket(NLTcpSocket* socket);
    void closeConnection();

signals:
    void sendMessage(const QString& msg);
    void sendMessageBinary(const QByteArray& msg);
    void isClientConnected(const bool& clientConnected);
};

#endif
