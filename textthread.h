#ifndef TEXTTHREAD_H
#define TEXTTHREAD_H

#include <QObject>

class TextThread : public QObject
{
    Q_OBJECT
public:
    TextThread(QObject *parent = nullptr);
    ~TextThread() {};

    void run();

//signals:

};

#endif // TEXTTHREAD_H
