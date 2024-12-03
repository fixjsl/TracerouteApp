#include "TracerouteApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TracerouteApp w;
    w.show();
    return a.exec();
}
