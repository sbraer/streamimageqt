#ifndef WAITKEYS_H
#define WAITKEYS_H

#include <QThread>
#include <iostream>
#include <QTextStream>

class waitKeys : public QThread
{
    Q_OBJECT
public:
    explicit waitKeys(QThread *parent = nullptr);

private:
    void run() override;
};

#endif // WAITKEYS_H