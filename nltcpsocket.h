#ifndef NLTCPSOCKET_H
#define NLTCPSOCKET_H

#include <QTcpSocket>

class NLTcpSocket final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(NLTcpSocket)

    private:
        QTcpSocket* m_pQTcpSocket;

    public:
        NLTcpSocket(QTcpSocket* socket);
        ~NLTcpSocket() override;
        QString getData();
        void write(QString data);

    private slots:
        void slotReadyRead ();
        void slotConnected ();
        void slotDisconnected();

    public slots:
        void writeMessage(const QString& msg);
        void writeMessageBinary(const QByteArray& msg);

    signals:
        void dataReady (NLTcpSocket *socket);
        void socketConnected (NLTcpSocket *socket);
        void socketDisconnected (NLTcpSocket *socket);
};
#endif
