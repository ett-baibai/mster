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

    m_tcpPort = 6666;
    m_udpPort = 8888;

    m_pTcpServer = NULL;
    m_pTcpSocket = NULL;
    m_pUdpSocket = new QUdpSocket;

    m_pTcpServer = new QTcpServer(this);
    m_pTcpServer->listen(QHostAddress::Any,m_tcpPort);

    //有新连接
    QObject::connect(m_pTcpServer,&QTcpServer::newConnection,
                     this, &networkNaster::on_OneClientListend);

    m_Timer=new QTimer;
    QObject::connect(m_Timer, SIGNAL(timeout()),
            this, SLOT(on_TimerOutToAutoSendUdpMsg()));

    m_isTimerBtnClicked = false;

}

networkNaster::~networkNaster()
{
    delete this->m_pTcpSocket;
    delete this->m_pUdpSocket;
    delete this->m_pTcpServer;
    delete this->m_Timer;
    delete ui;
}

void networkNaster::on_OneClientListend()
{
    m_pTcpSocket = m_pTcpServer->nextPendingConnection();
    //获取客户端的IP和端口
    ui->MessageList->addItem(m_pTcpSocket->peerAddress().toString() + " " +
                             QString::number(m_pTcpSocket->peerPort()) + " 连接成功，socket: " +
                             QString::number(m_pTcpServer->socketDescriptor()));
    //检测客户端的断开
    QObject::connect(m_pTcpSocket,&QTcpSocket::disconnected,
                     this, &networkNaster::on_OneClientDisconnect);

    QObject::connect(m_pTcpSocket,&QTcpSocket::readyRead,
            this, &networkNaster::on_ShowClientMsgFrom);
    /*
    QThread *subThread = new QThread;
    MultiThread *taskThread = new MultiThread;
    taskThread->moveToThread(subThread);
    connect(this,&networkNaster::s_SubThreadStart,
            taskThread, &MultiThread::on_RecvData);

    emit s_SubThreadStart(m_pTcpSocket);
    subThread->start();

    QObject::connect(taskThread, &MultiThread::s_sendMsg,
                     this, &networkNaster::on_ShowClientMsg);
                     */
}

void networkNaster::on_OneClientDisconnect()
{
    QString hostAddress=m_pTcpSocket->QAbstractSocket::peerAddress().toString();
    qDebug()<<"客户端 "<<hostAddress<<" 断开连接";
    m_pTcpSocket->close();
    m_pUdpSocket->close();
}

void networkNaster::on_ShowClientMsgFrom()
{
    QByteArray array = m_pTcpSocket->readAll();
    ui->MessageList->addItem(array);
}

void networkNaster::on_ShowClientMsgFromOtherThread(QByteArray array)
{
    ui->MessageList->addItem(array);
}

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
    qint64 WriteResult = m_pTcpSocket->write(str.toUtf8().data());

    bool bFlush = m_pTcpSocket->flush();

    if(WriteResult != -1 && bFlush == 1)//judge send succeedly or not
    {
        if(WriteResult == 0)
            QMessageBox::information(this,"error",tr("WriteResult = 0"));
        else
        {
        }
    }
    else
        qDebug()<<"failed to write!";
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
    m_pUdpSocket->writeDatagram("send me data",QHostAddress("127.0.0.1"), m_udpPort);
}

void networkNaster::on_ClearBtn_clicked()
{
    ui->MessageList->clear();
}

