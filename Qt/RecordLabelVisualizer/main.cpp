#include "recordlist.h"
#include "httpwindow.h"

#include <QApplication>

#include <QtNetwork>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RecordList w;
    w.show();
    return a.exec();
}
