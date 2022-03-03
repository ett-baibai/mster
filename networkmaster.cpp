#include "networkmaster.h"
#include "ui_networkmaster.h"
#include <QMessageBox>
#include <QtDebug>
#include <QThread>
#include "multithread.h"
#include <QDateTime>

#define mydebug qDebug()<< QDateTime::currentDateTime().toString("hh:mm:ss")<< ":"

networkNaster::networkNaster(QWidget *parent)
    : QDialog(parent), ui(new Ui::networkNaster), m_tcpPort(6666)
{
    ui->setupUi(this);

    m_pTcpServer = NULL;
    m_pTcpSocket = NULL;
    m_pTcpServer = new QTcpServer(this);
    m_pTcpServer->listen(QHostAddress::Any, m_tcpPort);

    //catch new connecting require
    QObject::connect(m_pTcpServer,&QTcpServer::newConnection, this, &networkNaster::on_OneClientListend);

    m_recvDataNum = 1;
    m_recvRawDataCache.clear();
    m_recombinedDataQueue.clear();

    //save data
    m_dataFile = new QFile("data.csv");
    m_saveDataTimer = new QTimer;
    QObject::connect(m_saveDataTimer, SIGNAL(timeout()), this, SLOT(on_TimerOutToSaveData()));

    //paint
    m_paintWidget = new paintWidget;
    QObject::connect(this, &networkNaster::s_PaintPoint, m_paintWidget, &paintWidget::on_PaintPoint);
}

networkNaster::~networkNaster()
{
    if(m_saveDataTimer->isActive())m_saveDataTimer->stop();
    delete m_saveDataTimer;
    delete m_pTcpServer;
    delete m_dataFile;
    delete m_paintWidget;
    delete ui;
}

void networkNaster::on_OneClientListend()
{
    m_pTcpSocket = m_pTcpServer->nextPendingConnection();
    ui->MessageList->addItem(m_pTcpSocket->peerAddress().toString() + " " +
                             QString::number(m_pTcpSocket->peerPort()) + " connected, socket: " +
                             QString::number(m_pTcpServer->socketDescriptor()));

    QObject::connect(m_pTcpSocket,&QTcpSocket::disconnected, this, &networkNaster::on_OneClientDisconnect);
    QObject::connect(m_pTcpSocket,&QTcpSocket::readyRead, this, &networkNaster::on_HandleClientMsg);
    m_CreateCsvFile();
    //m_saveDataTimer->start(100);
    m_paintWidget->show();
}

void networkNaster::m_CreateCsvFile()
{
    if(!m_dataFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("csv文件创建失败，数据无法保存，可能是文件被其他程序占用"));
        //m_saveDataTimer->stop();
    }
    else
        m_dataFile->close();
}

void networkNaster::on_OneClientDisconnect()
{
    QString hostAddress=m_pTcpSocket->QAbstractSocket::peerAddress().toString();
    ui->MessageList->addItem("client " + hostAddress + " disconnected");
    m_pTcpSocket->close();
    m_saveDataTimer->stop();
    //m_paintWidget->hide();
}

void networkNaster::on_HandleClientMsg()
{
    QByteArray array = m_pTcpSocket->readAll();
    for(int i = 0; i< array.length(); i++)
    {
        m_recvRawDataCache.enqueue((unsigned char)array[i]);
        //qDebug()<<"en: "<<(unsigned char)(array[i]);
    }

    unsigned int data = 0, index = 0;
    unsigned char bit[4] = {0};
    while(m_recvRawDataCache.length() >= 4)
    {
        for(index = 0; index < 4; index++)
        {
            bit[index] = m_recvRawDataCache.dequeue();
        }
        data = bit[0] | (bit[1] << 8) | (bit[2] << 16) | (bit[3] << 24);
        //m_recombinedDataQueue.enqueue(data);
        //qDebug()<<"de: "<< data;
        if(m_recvDataNum >= 1)
        {
            m_recvDataNum = 1;
            emit s_PaintPoint(data);
        }
        else
        {
            m_recvDataNum++;
        }
    }
}

void networkNaster::on_TimerOutToSaveData()
{
    char ch[20] = "";
    if (m_dataFile->open(QIODevice::Append))
    {
        while(!m_recombinedDataQueue.isEmpty())
        {
            sprintf(ch, "%d\n", m_recombinedDataQueue.dequeue());
            m_dataFile->write(ch);
        }
        m_dataFile->close();
    }
}

void networkNaster::on_ClearBtn_clicked()
{
    ui->MessageList->clear();
}

void networkNaster::on_paintWidgetBtn_clicked()
{
    m_paintWidget->show();
    unsigned int num[201] = {0};
    for(unsigned int i = 0, j = 8388708; i < 100; i++)
    {
        num[i] = j;
        j--;
    }

    for(unsigned int i = 100; i < 201; i++)
    {
        num[i] = i - 100;
    }

    unsigned int data = 0;
    for(unsigned int i = 0; i < 201; i++)
    {
        for(unsigned int ch = 0; ch < 3; ch++)
        {
            data = (ch << 24) | num[i];
            //qDebug()<<"1: "<< ch;
            emit s_PaintPoint(data);
        }
    }

}
