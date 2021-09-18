#include "waitkeys.h"
#include <QDebug>

waitKeys::waitKeys(QThread *parent) : QThread(parent)
{}

void waitKeys::run()
{
    qDebug() << "Press 'ENTER' to exit...";
    QTextStream stream(stdin);
    stream.readLine();
}
