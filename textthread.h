#ifndef TEXTTHREAD_H
#define TEXTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>


class TextThread : public QThread
{
    Q_OBJECT

public:
    TextThread(QObject *parent = nullptr);
    ~TextThread();

    void processText(const QString &fileName);
    // void getProgress();

signals:
    void progressChanged(const QStringList &words, const QStringList &values, int progress);
    void textProcessed();

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;
    bool abort;
    //bool timeout;
    QString fileName;
};

#endif // TEXTTHREAD_H
