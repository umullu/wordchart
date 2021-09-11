#include <QCoreApplication>

#include "textthread.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    TextThread thread;
    Logger logger;

    QObject::connect(&thread, &TextThread::progressChanged, &logger, &Logger::updateData);
    QObject::connect(&thread, &TextThread::finished, &app, &QCoreApplication::quit);

    thread.processText("rigveda_.txt");

    int res = app.exec();

    thread.quit();
    thread.wait();

    return res;
}
