#ifndef WAITKEYS_H
#define WAITKEYS_H

#include <QThread>
#include <QTextStream>

class WaitKeys final : public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(WaitKeys)

public:
    explicit WaitKeys(QThread *parent = nullptr);
    ~WaitKeys() override;

private:
    void run() override;
};

#endif // WAITKEYS_H
