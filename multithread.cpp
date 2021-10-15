#include "multithread.h"
#include <QDebug>


MultiThread::MultiThread(QObject *parent) : QObject(parent)
{
    m_pTcpSocket = NULL;
}

void MultiThread::on_RecvData(QTcpSocket *pTcpSocket)
{
    qDebug() << "当前线程对象的地址: " << QThread::currentThread();
    m_pTcpSocket = pTcpSocket;
    connect(pTcpSocket,&QTcpSocket::readyRead,
            this, &MultiThread::on_GetOneClientMsg);
}

void MultiThread::on_GetOneClientMsg()
{
    //从通信套接字中取出内容
    QByteArray array = m_pTcpSocket->readAll();
    emit s_sendMsg(array);
}
