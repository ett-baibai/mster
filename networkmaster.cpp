#include "networkmaster.h"
#include "ui_networkmaster.h"
#include <QMessageBox>
#include <QtDebug>
#include <QThread>
#include "multithread.h"
#include <QDateTime>

#define mydebug qDebug()<< QDateTime::currentDateTime().toString("hh:mm:ss")<< ":"

networkNaster::networkNaster(QWidget *parent)
    : QDialog(parent), ui(new Ui::networkNaster), m_tcpPort(6666), m_udpPort(8888)
{
    ui->setupUi(this);
    ui->ConnectBtn->setEnabled(false);
    ui->UdpSendOnceBtn->setEnabled(false);
    ui->UdpAutoSendBtn->setEnabled(false);

    m_arr ="send me data\n";

    for(int i = 0; i < 100; i++)
    {
        m_arr += "send me data\n";
    }

    mydebug << "主线程的地址: " << QThread::currentThread();

    m_pTcpServer = NULL;
    m_pTcpSocket = NULL;
    m_pUdpSocket = new QUdpSocket;

    m_pTcpServer = new QTcpServer(this);
    m_pTcpServer->listen(QHostAddress::Any,m_tcpPort);

    //有新连接
    QObject::connect(m_pTcpServer,&QTcpServer::newConnection,
                     this, &networkNaster::on_OneClientListend);


    m_TcpTimer = new QTimer;
    QObject::connect(m_TcpTimer, SIGNAL(timeout()),
            this, SLOT(on_TimerOutToAutoSendTcpMsg()));
    m_isTcpTimerBtnClicked = false;
    m_tcpSendIndex = 1;

    m_UdpTimer = new QTimer;
    QObject::connect(m_UdpTimer, SIGNAL(timeout()),
            this, SLOT(on_TimerOutToAutoSendUdpMsg()));
    m_isUdpTimerBtnClicked = false;

    m_indexDataArry = 0;

    m_paintWidget = new paintWidget;
    QObject::connect(this, &networkNaster::s_PaintPoint,
                     m_paintWidget, &paintWidget::on_PaintPoint);
}

networkNaster::~networkNaster()
{
    delete this->m_pTcpSocket;
    delete this->m_pUdpSocket;
    delete this->m_pTcpServer;
    delete this->m_TcpTimer;
    delete this->m_UdpTimer;
    delete m_paintWidget;
    delete ui;
}

void networkNaster::on_OneClientListend()
{
    m_pTcpSocket = m_pTcpServer->nextPendingConnection();
    //获取客户端的IP和端口
    ui->MessageList->addItem(m_pTcpSocket->peerAddress().toString() + " " +
                             QString::number(m_pTcpSocket->peerPort()) + " 连接成功，socket: " +
                             QString::number(m_pTcpServer->socketDescriptor()));
    //监测客户端的断开
    QObject::connect(m_pTcpSocket,&QTcpSocket::disconnected,
                     this, &networkNaster::on_OneClientDisconnect);

    //有数据就读取
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
    ui->MessageList->addItem("客户端 " + hostAddress + " 断开连接");
    m_pTcpSocket->close();
    m_pUdpSocket->close();
}

void networkNaster::on_ShowClientMsgFrom()
{
    QByteArray array = m_pTcpSocket->readAll();
    size_t length = array.length();
    for(size_t i= 0; i < length; i++)
    {
        qDebug()<<(unsigned char)(array[i]);
        if(m_indexDataArry < m_NumDataArry)
        {
            m_saveDataArry[m_indexDataArry] = (unsigned char)(array[i]);
            m_indexDataArry++;
        }
    }

    if(m_indexDataArry == m_NumDataArry)
    {
        m_indexDataArry++;
        m_paintWidget->show();
        emit s_PaintPoint(m_saveDataArry);//存满m_NumDataArry个数据就绘
    }
    //ui->MessageList->addItem(array);
}

void networkNaster::on_ShowClientMsgFromOtherThread(QByteArray array)
{
    ui->MessageList->addItem(array);
}

void networkNaster::on_ConnectBtn_clicked()
{
    QMessageBox::information(this, "title", "wiating for adding code");
}

void networkNaster::on_TcpSendOnceBtn_clicked()
{
    if(NULL == m_pTcpSocket)
    {
        QMessageBox::information(this, "err", "no tcp connection");
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

void networkNaster::on_TcpAutoSendBtn_clicked()
{
    if(false == m_isTcpTimerBtnClicked)
    {
        if(NULL == m_pTcpSocket)
        {
            QMessageBox::information(this, "err", "no tcp connection");
            return;
        }
        mydebug<<("定时器启动");
        ui->TcpAutoSendBtn->setText("transfering");
        m_TcpTimer->start(1000);
        m_isTcpTimerBtnClicked = true;
    }

    else
    {
        mydebug<<("定时器关闭");
        ui->TcpAutoSendBtn->setText("TcpAutoSend");
        m_TcpTimer->stop();
        m_isTcpTimerBtnClicked = false;
    }
}

void networkNaster::on_TimerOutToAutoSendTcpMsg()
{
    char dataStr[8] = {0};
    snprintf(dataStr, 8, "%d#", m_tcpSendIndex);
    m_pTcpSocket->write(dataStr);
    //qDebug()<<"send"<<dataStr;
    if(m_tcpSendIndex < 255)m_tcpSendIndex++;
    else m_tcpSendIndex = 1;
}

void networkNaster::on_UdpSendOnceBtn_clicked()
{
    mydebug<<"手动发送一次udp数据";
    UdpSendMsg();
}

void networkNaster::on_UdpAutoSendBtn_clicked()
{
    if(false == m_isUdpTimerBtnClicked)
    {
        mydebug<<("定时器启动");
        ui->UdpAutoSendBtn->setText("transfering");
        m_UdpTimer->start(2000);
        m_isUdpTimerBtnClicked = true;
    }

    else
    {
        mydebug<<("定时器关闭");
        ui->UdpAutoSendBtn->setText("UdpAutoSend");
        m_UdpTimer->stop();
        m_isUdpTimerBtnClicked = false;
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

    m_pUdpSocket->writeDatagram(m_arr,QHostAddress("127.0.0.1"), m_udpPort);
}

void networkNaster::on_ClearBtn_clicked()
{
    ui->MessageList->clear();
}

void networkNaster::on_paintWidgetBtn_clicked()
{
    m_paintWidget->show();
}

