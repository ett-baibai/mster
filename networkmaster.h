void on_SendBtn_clicked();
#ifndef NETWORKMASTER_H
#define NETWORKMASTER_H

#include <QDialog>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通信套接字

QT_BEGIN_NAMESPACE
namespace Ui { class networkNaster; }
QT_END_NAMESPACE

class networkNaster : public QDialog
{
    Q_OBJECT

public:
    networkNaster(QWidget *parent = nullptr);
    ~networkNaster();

signals:
    void OneClientConnected();

private slots:
    void on_ConnectBtn_clicked();
    void on_SendBtn_clicked();
    void on_OneClientListend();
    void on_GetOneClientMsg();

private:
    Ui::networkNaster *ui;
    QTcpServer *m_pTcpServer;//监听套接字
    QTcpSocket *m_pTcpSocket;//通信套接字
};
#endif // NETWORKMASTER_H
