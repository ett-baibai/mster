/********************************************************************************
** Form generated from reading UI file 'networkmaster.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETWORKMASTER_H
#define UI_NETWORKMASTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_networkNaster
{
public:
    QPushButton *ConnectBtn;
    QTextEdit *SendMsgEdit;
    QPushButton *TcpSendBtn;
    QListWidget *MessageList;
    QPushButton *UdpSendOnceBtn;
    QPushButton *UdpAutoSendBtn;
    QPushButton *ClearBtn;

    void setupUi(QDialog *networkNaster)
    {
        if (networkNaster->objectName().isEmpty())
            networkNaster->setObjectName(QString::fromUtf8("networkNaster"));
        networkNaster->resize(598, 254);
        ConnectBtn = new QPushButton(networkNaster);
        ConnectBtn->setObjectName(QString::fromUtf8("ConnectBtn"));
        ConnectBtn->setGeometry(QRect(110, 210, 71, 31));
        SendMsgEdit = new QTextEdit(networkNaster);
        SendMsgEdit->setObjectName(QString::fromUtf8("SendMsgEdit"));
        SendMsgEdit->setGeometry(QRect(390, 10, 181, 71));
        TcpSendBtn = new QPushButton(networkNaster);
        TcpSendBtn->setObjectName(QString::fromUtf8("TcpSendBtn"));
        TcpSendBtn->setGeometry(QRect(419, 89, 91, 31));
        MessageList = new QListWidget(networkNaster);
        MessageList->setObjectName(QString::fromUtf8("MessageList"));
        MessageList->setGeometry(QRect(20, 10, 341, 191));
        UdpSendOnceBtn = new QPushButton(networkNaster);
        UdpSendOnceBtn->setObjectName(QString::fromUtf8("UdpSendOnceBtn"));
        UdpSendOnceBtn->setGeometry(QRect(419, 138, 91, 31));
        UdpAutoSendBtn = new QPushButton(networkNaster);
        UdpAutoSendBtn->setObjectName(QString::fromUtf8("UdpAutoSendBtn"));
        UdpAutoSendBtn->setGeometry(QRect(419, 189, 91, 31));
        ClearBtn = new QPushButton(networkNaster);
        ClearBtn->setObjectName(QString::fromUtf8("ClearBtn"));
        ClearBtn->setGeometry(QRect(190, 210, 131, 31));

        retranslateUi(networkNaster);

        QMetaObject::connectSlotsByName(networkNaster);
    } // setupUi

    void retranslateUi(QDialog *networkNaster)
    {
        networkNaster->setWindowTitle(QCoreApplication::translate("networkNaster", "networkNaster", nullptr));
        ConnectBtn->setText(QCoreApplication::translate("networkNaster", "Connect", nullptr));
        TcpSendBtn->setText(QCoreApplication::translate("networkNaster", "TcpSend", nullptr));
        UdpSendOnceBtn->setText(QCoreApplication::translate("networkNaster", "UdpSendOnce", nullptr));
        UdpAutoSendBtn->setText(QCoreApplication::translate("networkNaster", "UdpAutoSend", nullptr));
        ClearBtn->setText(QCoreApplication::translate("networkNaster", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class networkNaster: public Ui_networkNaster {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETWORKMASTER_H
