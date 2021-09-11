#include <QDebug>

#include "logger.h"


Logger::Logger(QObject *parent) : QObject(parent)
{
}

void Logger::updateData(const QStringList &words, const QStringList &values, int progress)
{
    QString data;
    for (int i = 0; i < words.size(); ++i)
         data += words.at(i) + ":" + values.at(i) + " ";
    qDebug() << QString::number(progress) << "  " << data << "\n";
}

