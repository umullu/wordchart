#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);

public slots:
    void updateData(const QStringList &words, const QStringList &values, int progress);

};

#endif // LOGGER_H
