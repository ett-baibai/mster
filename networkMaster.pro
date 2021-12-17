QT       += core gui
QT       += network
QT       += charts
TARGET = Tcp-socket
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    multithread.cpp \
    networkmaster.cpp \
    paintWidget.cpp

HEADERS += \
    multithread.h \
    networkmaster.h \
    paintWidget.h

FORMS += \
    networkmaster.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
