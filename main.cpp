#include <QCoreApplication>

#include "textthread.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TextThread t;
    t.run();

    return 1;// a.exec();
}
