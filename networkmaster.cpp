#include "networkmaster.h"
#include "ui_networkmaster.h"
#include <QMessageBox>
#include <QtDebug>

networkNaster::networkNaster(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::networkNaster)
{
    ui->setupUi(this);

    m_pTcpServer = NULL;
    m_pTcpSocket = NULL;

    m_pTcpServer = new QTcpServer(this);
    m_pTcpServer->listen(QHostAddress::Any,8888);
    QObject::connect(m_pTcpServer,&QTcpServer::newConnection,
                     this, &networkNaster::on_OneClientListend);
}

networkNaster::~networkNaster()
{
    m_pTcpServer->close();
    m_pTcpServer->deleteLater();
    delete m_pTcpServer;
    delete m_pTcpSocket;
    delete ui;
}

void networkNaster::on_OneClientListend()
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

void networkNaster::on_GetOneClientMsg()
{
    //从通信套接字中取出内容
    QByteArray array = m_pTcpSocket->readAll();
    ui->IPAddrBrowser->append(array);
}

void networkNaster::on_ConnectBtn_clicked()
{
    //ui->IPAddrBrowser->insertPlainText("192.168.1.1");
    QMessageBox::information(this, "title", "wiating for adding");
}

void networkNaster::on_SendBtn_clicked()
{
    if(NULL == m_pTcpSocket)
    {
        QMessageBox::information(this, "err", "no connection");
        return;
    }
    QString str = ui->SendMsgEdit->toPlainText();
    m_pTcpSocket->write(str.toUtf8().data());
}
