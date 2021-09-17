#ifndef MYTIMER_H
#define MYTIMER_H

#include <QThread>
#include <iostream>
#include <QTimer>
#include <QFile>
#include "imagecreator.h"

class MyTimer final : public QThread
{
    Q_OBJECT
private:
    Q_DISABLE_COPY_MOVE(MyTimer)

    bool _clientConnected;
    ImageCreator _imgCreator;

public:
    explicit MyTimer(QObject *parent = nullptr);
    ~MyTimer() override;
    void run() override;

public slots:
    void writeInfo();
    void isClientConnected(const bool& clientConnected);

signals:
    void sendMessage(const QString& msg);
    void sendMessageBinary(const QByteArray& msg);
};

#endif // MYTIMER_H
