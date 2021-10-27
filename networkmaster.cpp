#include "networkmaster.h"
#include "ui_networkmaster.h"
#include <QMessageBox>
#include <QtDebug>
#include <QThread>
#include "multithread.h"
#include <QDateTime>

#define mydebug qDebug()<< QDateTime::currentDateTime().toString("hh:mm:ss")<< ":"

networkNaster::networkNaster(QWidget *parent)
    : QDialog(parent), ui(new Ui::networkNaster)
{
    ui->setupUi(this);
    ui->ConnectBtn->setEnabled(false);
    mydebug << "主线程的地址: " << QThread::currentThread();

    m_pTcpServer = NULL;
    m_pTcpSocket = NULL;
    m_pUdpSocket = new QUdpSocket;

    m_pTcpServer = new QTcpServer(this);
    m_pTcpServer->listen(QHostAddress::Any,8888);

    //有新连接
    QObject::connect(m_pTcpServer,&QTcpServer::newConnection,
                     this, &networkNaster::on_OneClientListend);

    m_Timer=new QTimer;
    QObject::connect(m_Timer, SIGNAL(timeout()),
            this, SLOT(on_TimerOutToAutoSendUdpMsg()));

    m_isTimerBtnClicked = false;

    //连接出错，（暂未实现）
    //QObject::connect(m_pTcpServer, SIGNAL(error(QAbstractSocket::SocketError)),
    //                 this, SLOT(on_MSGError(QAbstractSocket::SocketError)));
}

networkNaster::~networkNaster()
{
    m_pTcpServer->close();
    m_pTcpServer->deleteLater();

    m_pUdpSocket->close();
    m_pUdpSocket->deleteLater();

    delete this->m_pTcpServer;
    delete this->m_pTcpSocket;
    delete this->m_pUdpSocket;
    delete this->m_Timer;
    delete ui;
}

void networkNaster::on_OneClientListend()
{
    m_pTcpSocket = m_pTcpServer->nextPendingConnection();
    //获取对方的IP和端口
    ui->MessageList->addItem(m_pTcpSocket->peerAddress().toString() + " " +
                             QString::number(m_pTcpSocket->peerPort()) + " 连接成功，socket: " +
                             QString::number(m_pTcpServer->socketDescriptor()));

    QThread *subThread = new QThread;
    MultiThread *taskThread = new MultiThread;
    taskThread->moveToThread(subThread);
    connect(this,&networkNaster::s_SubThreadStart,
            taskThread, &MultiThread::on_RecvData);

    emit s_SubThreadStart(m_pTcpSocket);
    subThread->start();

    QObject::connect(taskThread, &MultiThread::s_sendMsg,
                     this, &networkNaster::on_ShowClientMsg);
}

void networkNaster::on_ShowClientMsg(QByteArray array)
{
    ui->MessageList->addItem(array);
}
//void networkNaster::on_GetOneClientMsg()
/*
{
    //从通信套接字中取出内容
    QByteArray array = m_pTcpSocket->readAll();
    ui->MessageList->addItem(array);
}
*/

void networkNaster::on_ConnectBtn_clicked()
{
    QMessageBox::information(this, "title", "wiating for adding");
}

void networkNaster::on_TcpSendBtn_clicked()
{
    if(NULL == m_pTcpSocket)
    {
        QMessageBox::information(this, "err", "no connection");
        return;
    }
    QString str = ui->SendMsgEdit->toPlainText();
    m_pTcpSocket->write(str.toUtf8().data());
}

void networkNaster::on_MSGError(QAbstractSocket::SocketError)
{
    switch(m_pTcpSocket->error())
    {
        case QAbstractSocket::RemoteHostClosedError://客户端断开
        {
            QString hostAddress=m_pTcpSocket->QAbstractSocket::peerAddress().toString();
            qDebug()<<"客户端 "<<hostAddress<<" 断开连接";
            break;
        }
        default:
        {
            QMessageBox::information(this, "show", m_pTcpSocket->errorString());
            break;
        }
    }
}

void networkNaster::on_UdpSendOnceBtn_clicked()
{
    mydebug<<"手动发送一次udp数据";
    UdpSendMsg();
}

void networkNaster::on_UdpAutoSendBtn_clicked()
{
    if(false == m_isTimerBtnClicked)
    {
        mydebug<<("定时器启动");
        ui->UdpAutoSendBtn->setText("transfering");
        m_Timer->start(2000);
        m_isTimerBtnClicked = true;
    }

    else
    {
        mydebug<<("定时器关闭");
        ui->UdpAutoSendBtn->setText("UdpAutoSend");
        m_Timer->stop();
        m_isTimerBtnClicked = false;
    }
}

void networkNaster::on_TimerOutToAutoSendUdpMsg()
{
    mydebug<<"自动发送一次udp数据";
    UdpSendMsg();
}

void networkNaster::UdpSendMsg()
{
    //QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_pUdpSocket->writeDatagram("send me data",QHostAddress("127.0.0.1"), 2333);
}

void networkNaster::on_pushButton_clicked()
{
    ui->MessageList->clear();
}

