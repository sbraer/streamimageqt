#include "mytimer.h"
#include <QDateTime>

MyTimer::MyTimer(QObject *parent) : QThread(parent), m_bClientConnected(false)
{
    qDebug("Constructor MyTimer");
}

MyTimer::~MyTimer()
{
    qDebug("Destructor MyTimer");
}

void MyTimer::run()
{
    QTimer timer;
    connect(&timer, &QTimer::timeout, this, &MyTimer::writeInfo);
    timer.start(1000);
    exec();
    timer.stop();
}

void MyTimer::writeInfo()
{
    if (m_bClientConnected) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        QByteArray blob = m_pImageCreator.createImage();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        QString memo_text="\r\n--boundary\r\nContent-Type: image/jpeg\r\n"
                "Content-Length: %1\r\n\r\n";
        auto textToSend = memo_text.arg(blob.size()).toStdString();
        QByteArray textTemp(textToSend.c_str(), static_cast<long>(textToSend.length()));
        textTemp.append(blob);
        qInfo() << "Send " << textTemp.size() << " "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                  << "ms";
        emit sendMessageBinary(textTemp);
    }
    else {
        qDebug("No client connected");
    }
}

void MyTimer::isClientConnected(const bool& clientConnected)
{
    m_bClientConnected = clientConnected;
}
