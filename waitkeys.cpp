#include "waitkeys.h"
#include <QDebug>

WaitKeys::WaitKeys(QThread *parent) : QThread{parent}
{
    qDebug("Constructor WaitKeys");
}

WaitKeys::~WaitKeys()
{
    qDebug("Deconstructor WaitKeys");
}

void WaitKeys::run()
{
    qInfo() << "Press 'ENTER' to exit...";
    QTextStream stream{stdin};
    stream.readLine();
}
