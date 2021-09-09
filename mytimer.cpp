#include "mytimer.h"
#include <QDateTime>

MyTimer::MyTimer(QObject *parent) : QThread(parent)
{}

MyTimer::~MyTimer()
{}

void MyTimer::run()
{
    _clientConnected = false;
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, this, &MyTimer::writeInfo);
    timer.start(1000);
    exec();
}

void MyTimer::writeInfo()
{
    if (_clientConnected) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        auto blob = _imgCreator.createImage();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        QString memo_text="\r\n--boundary\r\nContent-Type: image/jpeg\r\n"
                "Content-Length: %1\r\n\r\n";
        auto textToSend = memo_text.arg(blob.size()).toStdString();
        QByteArray textTemp(textToSend.c_str(), static_cast<long>(textToSend.length()));
        textTemp.append(blob);
        std::cout << "Send " << textTemp.size() << " "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                  << "ms\n";
        emit sendMessageBinary(textTemp);
    }
    else {
#ifdef QT_DEBUG
        std::cout << "No client connected\n";
#endif
    }
}

void MyTimer::isClientConnected(const bool& clientConnected)
{
    _clientConnected = clientConnected;
}
