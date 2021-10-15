#include "networkmaster.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    networkNaster w;
    w.show();
    return a.exec();
}
