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
    Q_PROPERTY(QStringList words READ getWords NOTIFY dataUpdated)
    Q_PROPERTY(QVariantList values READ getValues NOTIFY dataUpdated)
    Q_PROPERTY(int maximum READ getMaximum NOTIFY dataUpdated)

public:
    TextThread(QObject *parent = nullptr);
    ~TextThread();

    Q_INVOKABLE void processText();
    Q_INVOKABLE void cancelProcessing();
    //void updateData();
    void setPath(const QUrl &path);
    int getProgress() const;
    QStringList getWords() const;
    QVariantList getValues() const;
    int getMaximum() const;

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
    QStringList words;
    QVariantList values;
    int maximum;

};

#endif // TEXTTHREAD_H
