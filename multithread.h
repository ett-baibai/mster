#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>

class MultiThread : public QObject
{
    Q_OBJECT
public:
    explicit MultiThread(QObject *parent = nullptr);

signals:
    void s_sendMsg(QByteArray array);

public slots:
    void on_RecvData(QTcpSocket *pTcpSocket);
    void on_GetOneClientMsg();

private:
    QTcpSocket *m_pTcpSocket;

};

#endif // MULTITHREAD_H
