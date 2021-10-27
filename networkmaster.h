void on_SendBtn_clicked();
#ifndef NETWORKMASTER_H
#define NETWORKMASTER_H

#include <QDialog>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通信套接字
#include <QUdpSocket>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class networkNaster; }
QT_END_NAMESPACE

class networkNaster : public QDialog
{
    Q_OBJECT

public:
    networkNaster(QWidget *parent = nullptr);
    ~networkNaster();
    void UdpSendMsg();

signals:
    void OneClientConnected();
    void s_SubThreadStart(QTcpSocket *tcpSocket);

private slots:
    void on_ConnectBtn_clicked();
    void on_TcpSendBtn_clicked();
    void on_OneClientListend();
    //void on_GetOneClientMsg();
    void on_ShowClientMsg(QByteArray array);
    void on_MSGError(QAbstractSocket::SocketError);
    void on_UdpSendOnceBtn_clicked();
    void on_UdpAutoSendBtn_clicked();
    void on_pushButton_clicked();

    void on_TimerOutToAutoSendUdpMsg();

private:
    Ui::networkNaster *ui;
    QTcpServer *m_pTcpServer;//监听套接字
    QTcpSocket *m_pTcpSocket;//通信套接字
    QUdpSocket *m_pUdpSocket;

    QTimer *m_Timer;
    bool m_isTimerBtnClicked;

    //static const int maxThreadNum = 10;
};
#endif // NETWORKMASTER_H
