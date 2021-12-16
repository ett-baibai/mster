#include "multithread.h"
#include <QDebug>


MultiThread::MultiThread(QObject *parent) : QObject(parent)
{
    m_pTcpSocket = NULL;
}

MultiThread::~MultiThread()
{
}

void MultiThread::on_RecvData(QTcpSocket *pTcpSocket)
{
    qDebug() << "current thread address: " << QThread::currentThread();
    m_pTcpSocket = pTcpSocket;
    connect(pTcpSocket,&QTcpSocket::readyRead,
            this, &MultiThread::on_GetOneClientMsg);
}

void MultiThread::on_GetOneClientMsg()
{
    //get msg
    QByteArray array = m_pTcpSocket->readAll();
    emit s_sendMsg(array);
}
