#include <QApplication>
#include <cstring>
#include "tcpserver.h"
#include "waitkeys.h"
#include "mytimer.h"

int main(int argc, char *argv[])
{
#ifdef __linux__
    //linux code goes here
    char** argv2 = new char*[3];
    size_t length = strlen(argv[0])+1;
    argv2[0] = new char[length];
    strncpy(argv2[0], argv[0], length);
    argv2[1] = const_cast<char*>("-platform");
    argv2[2] = const_cast<char*>("offscreen");
    argc = 3;
    QApplication a(argc, argv2);
#else
    //windows code goes here
    QApplication a(argc, argv);
#endif
    TcpServer tcpServer;
    waitKeys wk;
    MyTimer mt;

    QObject::connect(&wk, &waitKeys::finished, &tcpServer, &TcpServer::closeConnection);
    QObject::connect(&wk, &waitKeys::finished, &mt, &MyTimer::quit);
    QObject::connect(&mt, &MyTimer::sendMessage, &tcpServer, &TcpServer::sendMessage, Qt::ConnectionType::QueuedConnection);
    QObject::connect(&mt, &MyTimer::sendMessageBinary, &tcpServer, &TcpServer::sendMessageBinary, Qt::ConnectionType::QueuedConnection);
    QObject::connect(&tcpServer, &TcpServer::isClientConnected, &mt, &MyTimer::isClientConnected, Qt::ConnectionType::QueuedConnection);
    QObject::connect(&wk, &waitKeys::finished, &a, &QApplication::quit);

    // START NOT RUN!!!
    wk.start();
    mt.start();

    int value = a.exec();
    wk.wait();
    mt.wait();
    return value;
}
