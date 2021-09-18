#ifndef WAITKEYS_H
#define WAITKEYS_H

#include <QThread>
#include <QTextStream>

class waitKeys final : public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(waitKeys)

public:
    explicit waitKeys(QThread *parent = nullptr);

private:
    void run() override;
};

#endif // WAITKEYS_H
