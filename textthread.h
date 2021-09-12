#ifndef TEXTTHREAD_H
#define TEXTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QVariantMap>
#include <QUrl>


class TextThread : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QUrl path WRITE setPath)
    Q_PROPERTY(int progress READ getProgress NOTIFY dataUpdated)
    Q_PROPERTY(QVariantMap words READ getWords NOTIFY dataUpdated)

public:
    TextThread(QObject *parent = nullptr);
    ~TextThread();

    Q_INVOKABLE void processText();
    Q_INVOKABLE void cancelProcessing();
    //void updateData();
    void setPath(const QUrl &path);
    int getProgress() const;
    QVariantMap getWords() const;

signals:
    void dataUpdated();
    void textProcessed();

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;
    bool canceled;
    bool terminated;
    //bool timeout;
    QUrl path;
    int progress;
    QVariantMap words;
};

#endif // TEXTTHREAD_H
