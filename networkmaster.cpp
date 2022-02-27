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
    //mydebug << "main thread address: " << QThread::currentThread();

    m_pTcpServer = NULL;
    m_pTcpSocket = NULL;
    m_pUdpSocket = new QUdpSocket;

    m_pTcpServer = new QTcpServer(this);
    m_pTcpServer->listen(QHostAddress::Any,m_tcpPort);

    //catch new connecting require
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

    m_recvRawDataCache.clear();

    //paint
    //m_paintWidget = new paintWidget;
    //QObject::connect(this, &networkNaster::s_PaintPoint,
    //                 m_paintWidget, &paintWidget::on_PaintPoint);
}

networkNaster::~networkNaster()
{
    delete this->m_pTcpSocket;
    delete this->m_pUdpSocket;
    delete this->m_pTcpServer;
    delete this->m_TcpTimer;
    delete this->m_UdpTimer;
    //delete m_paintWidget;
    delete ui;
}

void networkNaster::on_OneClientListend()
{
    m_pTcpSocket = m_pTcpServer->nextPendingConnection();
    ui->MessageList->addItem(m_pTcpSocket->peerAddress().toString() + " " +
                             QString::number(m_pTcpSocket->peerPort()) + " connected, socket: " +
                             QString::number(m_pTcpServer->socketDescriptor()));
    QObject::connect(m_pTcpSocket,&QTcpSocket::disconnected,
                     this, &networkNaster::on_OneClientDisconnect);

    QObject::connect(m_pTcpSocket,&QTcpSocket::readyRead,
            this, &networkNaster::on_HandleClientMsg);
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
    ui->MessageList->addItem("client " + hostAddress + " disconnected");
    m_pTcpSocket->close();
    m_pUdpSocket->close();

    if(ui->TcpAutoSendBtn->text() == "transfering")
    {
        mydebug<<("close tcp timer");
        ui->TcpAutoSendBtn->setText("TcpAutoSend");
        m_TcpTimer->stop();
        m_isTcpTimerBtnClicked = false;
    }
}

void networkNaster::on_HandleClientMsg()
{
    QByteArray array = m_pTcpSocket->readAll();
    for(int i = 0; i< array.length(); i++)
    {
        m_recvRawDataCache.enqueue((unsigned char)array[i]);
        qDebug()<<"en: "<<(unsigned char)(array[i]);
    }

    int data = 0, index = 0, bit[4] = {0};
    while(m_recvRawDataCache.length() >= 4)
    {
        for(index = 0, data = 0; index < 4; index++)
        {
            bit[index] = m_recvRawDataCache.dequeue();
        }
        data = bit[0] | (bit[1] << 8) | (bit[2] << 16) | (bit[3] << 24);
        qDebug()<<"de: "<< data;
    }

/*

    if(m_indexDataArry == m_DataArryNum)
    {
        m_indexDataArry++;
        m_paintWidget->show();
        emit s_PaintPoint(m_saveDataArry);//to paint
    }
    */
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
        mydebug<<("start tcp timer");
        ui->TcpAutoSendBtn->setText("transfering");
        m_TcpTimer->start(1000);
        m_isTcpTimerBtnClicked = true;
    }

    else
    {
        mydebug<<("close tcp timer");
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
    if(m_tcpSendIndex < 5)m_tcpSendIndex++;
    else m_tcpSendIndex = 1;
}

void networkNaster::on_UdpSendOnceBtn_clicked()
{
    mydebug<<"mannual send udp data once";
    UdpSendMsg();
}

void networkNaster::on_UdpAutoSendBtn_clicked()
{
    if(false == m_isUdpTimerBtnClicked)
    {
        mydebug<<("start udp timer");
        ui->UdpAutoSendBtn->setText("transfering");
        m_UdpTimer->start(2000);
        m_isUdpTimerBtnClicked = true;
    }

    else
    {
        mydebug<<("close udp timer");
        ui->UdpAutoSendBtn->setText("UdpAutoSend");
        m_UdpTimer->stop();
        m_isUdpTimerBtnClicked = false;
    }
}

void networkNaster::on_TimerOutToAutoSendUdpMsg()
{
    mydebug<<"atuo send udp data once";
    UdpSendMsg();
}

void networkNaster::UdpSendMsg()
{
    //QString time = QDateTime::currentDateTime().toString("hh:mm:ss");

    //m_pUdpSocket->writeDatagram(m_arr,QHostAddress("127.0.0.1"), m_udpPort);
}

void networkNaster::on_ClearBtn_clicked()
{
    ui->MessageList->clear();
}

void networkNaster::on_paintWidgetBtn_clicked()
{
    /*
    for(unsigned int i = 0; i < m_DataArryNum; i++)
    {
        m_saveDataArry[i] = (i + 1) % 256;
    }
    m_paintWidget->show();
    emit s_PaintPoint(m_saveDataArry);//to paint
    */
}
