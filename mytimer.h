#ifndef MYTIMER_H
#define MYTIMER_H

#include <QThread>
#include <QTimer>
#include <QFile>
#include "imagecreator.h"

class MyTimer final : public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MyTimer)

private:
    bool _clientConnected;
    ImageCreator _imgCreator;

public:
    explicit MyTimer(QObject *parent = nullptr);
    void run() override;

public slots:
    void writeInfo();
    void isClientConnected(const bool& clientConnected);

signals:
    void sendMessage(const QString& msg);
    void sendMessageBinary(const QByteArray& msg);
};

#endif // MYTIMER_H
