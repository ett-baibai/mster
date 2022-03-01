#ifndef NETWORKMASTER_H
#define NETWORKMASTER_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include <QQueue>
#include <QStringList>
#include <QFile>
#include "paintWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class networkNaster; }
QT_END_NAMESPACE

class networkNaster : public QDialog
{
    Q_OBJECT
public:
    networkNaster(QWidget *parent = nullptr);
    ~networkNaster();

private:
    Ui::networkNaster *ui;
    QTcpServer *m_pTcpServer;
    QTcpSocket *m_pTcpSocket;
    const unsigned short m_tcpPort;

    int m_recvDataNum;
    QQueue<unsigned char> m_recvRawDataCache;
    QQueue<int> m_recombinedDataQueue;
    QFile *m_dataFile;
    paintWidget *m_paintWidget;
    QTimer *m_saveDataTimer;

    void m_CreateCsvFile();

private slots:
    void on_OneClientListend();
    void on_OneClientDisconnect();
    void on_HandleClientMsg();
    void on_TimerOutToSaveData();
    void on_ClearBtn_clicked();
    void on_paintWidgetBtn_clicked();

signals:
    void OneClientConnected();
    void s_PaintPoint(int data);
};
#endif
