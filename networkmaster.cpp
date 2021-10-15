#include "networkmaster.h"
#include "ui_networkmaster.h"
#include <QMessageBox>
#include <QtDebug>

networkNaster::networkNaster(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::networkNaster)
{
    ui->setupUi(this);
    m_pQthread = NULL;
    m_pMultiThread = NULL;

    m_pTcpServer = NULL;
    m_pTcpSocket = NULL;

    m_pTcpServer = new QTcpServer(this);
    m_pTcpServer->listen(QHostAddress::Any,8888);
    QObject::connect(m_pTcpServer,&QTcpServer::newConnection,
                     this, &networkNaster::on_OpenThreadSlot);
}

networkNaster::~networkNaster()
{
    m_pTcpServer->close();
    m_pTcpServer->deleteLater();
    delete m_pTcpServer;
    delete m_pTcpSocket;
    delete ui;
}

void networkNaster::on_GetOneClientMsg()
{
    //从通信套接字中取出内容
    QByteArray array = m_pTcpSocket->readAll();
    ui->IPAddrBrowser->append(array);
}

void networkNaster::on_OpenThreadSlot()
{
    /*开启一条多线程*/
    qDebug()<<tr("开启线程");//线程容器
    m_pQthread = new QThread;
    m_pMultiThread = new MultiThread;
    m_pMultiThread->moveToThread(m_pQthread);//将创建的对象移到线程容器中
    connect(m_pQthread,SIGNAL(finished()),m_pMultiThread,SLOT(deleteLater())); //终止线程时要调用deleteLater槽函数
    connect(m_pQthread,SIGNAL(started()),this,SLOT(on_OneClientListened()));  //开启线程槽函数
    connect(m_pQthread,SIGNAL(finished()),this,SLOT(on_FinishedThreadSlot()));
    m_pQthread->start(); //开启多线程槽函数
    qDebug()<<"networkNaster QThread::currentThreadId()=="<<QThread::currentThreadId();
}

void networkNaster::on_OneClientListened()
{
    m_pTcpSocket = m_pTcpServer->nextPendingConnection();
    //获取对方的IP和端口
    QString ip = m_pTcpSocket->peerAddress().toString();
    qint16 port = m_pTcpSocket->peerPort();
    QString temp = QString("[%1:%2]:成功链接").arg(ip).arg(port);
    ui->IPAddrBrowser->insertPlainText(temp);
    connect(m_pTcpSocket,&QTcpSocket::readyRead,
            this, &networkNaster::on_GetOneClientMsg);
}

void networkNaster::on_CloseThreadSlot()
{
    qDebug()<<tr("关闭线程");
    if(m_pQthread->isRunning())
    {
        m_pMultiThread->CloseThread(); //关闭线程槽函数
        m_pQthread->quit();            //退出事件循环
        m_pQthread->wait();            //释放线程槽函数资源
    }
}

void networkNaster::on_FinishedThreadSlot()
{
    qDebug()<<tr("多线程触发了finished信号123");
}

void networkNaster::on_ConnectBtn_clicked()
{
    ui->IPAddrBrowser->insertPlainText("192.168.1.1");
    QMessageBox::information(this, "title", "OK");
}

void networkNaster::on_SendBtn_clicked()
{
    if(NULL == m_pTcpSocket)        //防止在未输入之前点击按键导致服务器没有收到数据而出现程序异常
    {
        return;
    }
    QString str = ui->SendMsgEdit->toPlainText();
    m_pTcpSocket->write(str.toUtf8().data());
}
