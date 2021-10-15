#include "multithread.h"
#include <QDebug>
#include <QThread>

MultiThread::MultiThread(QObject *parent) : QObject(parent)
{
    m_isStop = false;
}

void MultiThread::CloseThread()
{
    m_isStop = true;
}
