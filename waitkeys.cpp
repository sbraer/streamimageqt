#include "waitkeys.h"

waitKeys::waitKeys(QThread *parent) : QThread(parent)
{}

void waitKeys::run()
{
    std::cout << "Press 'ENTER' to exit...\n";
    QTextStream stream(stdin);
    stream.readLine();
}
