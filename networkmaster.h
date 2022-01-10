#ifndef NETWORKMASTER_H
#define NETWORKMASTER_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include <QQueue>
//#include "paintWidget.h"

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

private:
    Ui::networkNaster *ui;
    QTcpServer *m_pTcpServer;
    QTcpSocket *m_pTcpSocket;
    QUdpSocket *m_pUdpSocket;

    QTimer *m_TcpTimer;
    bool m_isTcpTimerBtnClicked;
    unsigned char m_tcpSendIndex;

    QTimer *m_UdpTimer;
    bool m_isUdpTimerBtnClicked;

    const unsigned short m_tcpPort;
    const unsigned short m_udpPort;

    QQueue<unsigned char> m_recvRawDataCache;

    //paintWidget *m_paintWidget;

private slots:
    void on_ConnectBtn_clicked();
    void on_TcpSendOnceBtn_clicked();
    void on_OneClientListend();
    void on_OneClientDisconnect();
    void on_ShowClientMsgFromOtherThread(QByteArray array);
    void on_HandleClientMsg();
    void on_UdpSendOnceBtn_clicked();
    void on_UdpAutoSendBtn_clicked();
    void on_ClearBtn_clicked();
    void on_TimerOutToAutoSendTcpMsg();
    void on_TimerOutToAutoSendUdpMsg();
    void on_TcpAutoSendBtn_clicked();
    void on_paintWidgetBtn_clicked();

signals:
    void OneClientConnected();
    void s_SubThreadStart(QTcpSocket *tcpSocket);
    void s_PaintPoint(QQueue<unsigned int> dataQueue);

};
#endif
