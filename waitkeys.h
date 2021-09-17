#ifndef WAITKEYS_H
#define WAITKEYS_H

#include <QThread>
#include <iostream>
#include <QTextStream>

class waitKeys final : public QThread
{
    Q_OBJECT
public:
    explicit waitKeys(QThread *parent = nullptr);

private:
    Q_DISABLE_COPY_MOVE(waitKeys)
    void run() override;
};

#endif // WAITKEYS_H
