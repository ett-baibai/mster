#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include <QObject>

class MultiThread : public QObject
{
    Q_OBJECT
public:
    explicit MultiThread(QObject *parent = nullptr);
    void CloseThread();
    volatile bool m_isStop;
};

#endif // MULTITHREAD_H
