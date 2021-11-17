void on_SendBtn_clicked();
#ifndef NETWORKMASTER_H
#define NETWORKMASTER_H

#include <QDialog>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通信套接字
#include <QUdpSocket>
#include <QTimer>
#include "paintWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class networkNaster; }
QT_END_NAMESPACE

class networkNaster : public QDialog
{
    Q_OBJECT

private:
    Ui::networkNaster *ui;
    QTcpServer *m_pTcpServer;//监听套接字
    QTcpSocket *m_pTcpSocket;//通信套接字
    QUdpSocket *m_pUdpSocket;

    QTimer *m_TcpTimer;
    bool m_isTcpTimerBtnClicked;
    unsigned char m_tcpSendIndex;

    QTimer *m_UdpTimer;
    bool m_isUdpTimerBtnClicked;

    const unsigned short m_tcpPort;
    const unsigned short m_udpPort;

    QByteArray m_arr;

    const static unsigned int m_NumDataArry = 2048;
    unsigned char m_saveDataArry[m_NumDataArry];
    unsigned int m_indexDataArry;

    paintWidget *m_paintWidget;
    //static const int maxThreadNum = 10;

private slots:
    void on_ConnectBtn_clicked();
    void on_TcpSendOnceBtn_clicked();
    void on_OneClientListend();
    void on_OneClientDisconnect();
    void on_ShowClientMsgFromOtherThread(QByteArray array);
    void on_ShowClientMsgFrom();
    void on_UdpSendOnceBtn_clicked();
    void on_UdpAutoSendBtn_clicked();
    void on_ClearBtn_clicked();
    void on_TimerOutToAutoSendTcpMsg();
    void on_TimerOutToAutoSendUdpMsg();
    void on_TcpAutoSendBtn_clicked();
    void on_paintWidgetBtn_clicked();

public:
    networkNaster(QWidget *parent = nullptr);
    ~networkNaster();
    void UdpSendMsg();

signals:
    void OneClientConnected();
    void s_SubThreadStart(QTcpSocket *tcpSocket);
    void s_PaintPoint(unsigned char m_saveDataArry[m_NumDataArry]);

};
#endif // NETWORKMASTER_H
