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
    QTimer::singleShot(0, this, &MyTimer::writeInfo);
    exec();
}

void MyTimer::writeInfo()
{
    if (m_bClientConnected) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        QByteArray blob = m_pImageCreator.createImage();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        QString memo_text="\r\n--boundary\r\nContent-Type: image/jpeg\r\n"
                "Content-Length: %1\r\n\r\n";
        std::string textToSend = memo_text.arg(blob.size()).toStdString();
        QByteArray textTemp(textToSend.c_str(), static_cast<long>(textToSend.length()));
        textTemp.append(blob);
        qInfo() << "Send " << textTemp.size() << " "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                  << "ms";
        emit sendMessageBinary(textTemp);
    }

    QDateTime dt = QDateTime::currentDateTimeUtc();
    QTime qt1 = dt.time();
    QTime qt2 = QTime(qt1.hour(), qt1.minute(), qt1.second(), 0).addSecs(1);
    int delay = qt1.msecsTo(qt2);
    qDebug("%dms: %s", delay, dt.toString("dd/MM/yyyy hh:mm:ss zzz UTC").toStdString().c_str());
    QTimer::singleShot(delay, this, &MyTimer::writeInfo);
}

void MyTimer::isClientConnected(const bool& clientConnected)
{
    m_bClientConnected = clientConnected;
}
