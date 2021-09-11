#include <QFile>
#include <QTextStream>
#include <QElapsedTimer>
#include <QRegularExpression>
#include <QDebug>

#include "textthread.h"

TextThread::TextThread(QObject *parent)
    : QThread(parent)
{
      abort = false;
      //timeout = false;
}

TextThread::~TextThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void TextThread::processText(const QString &fileName)
{
    QMutexLocker locker(&mutex);

    this->fileName = fileName;

    if (!isRunning())
        start();
}

/*
void TextThread::getProgress()
{
    timeout = true;
}
*/

void TextThread::run()
{
    const int ProgressInterval = 200000;
    const int MaxTopItems = 15;

    const bool CaseSensitivity = false;
    const bool ApostropheUsing = false;

    QRegularExpression re;
    if (!ApostropheUsing)
        re.setPattern("([A-za-zА-Яа-я]+)");
    else
        re.setPattern("([A-za-zА-Яа-я']+)");

    QHash<QString, int> hash;
    QMultiMap<int, QString> top;
    int minCount = 0;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream stream(&file);

    qint64 fileSize = file.size();
    qint64 prevProcessed = 0;
    qint64 processed = 0;

    //QTimer *timer = new QTimer(this);
    //connect(timer, &QTimer::timeout, this, &TextThread::getProgress);
    //timer->start(500);

    QElapsedTimer t;
    t.start();

    while (!stream.atEnd()) {

        QString str;
        stream >> str;

        // Remove backslashes
        str.remove(QChar('\\'), Qt::CaseInsensitive);

        // Change to lowercase
        if (!CaseSensitivity)
            str = str.toLower();

        // Extract words
        QRegularExpressionMatchIterator iter = re.globalMatch(str);
        while (iter.hasNext()) {
            QRegularExpressionMatch match = iter.next();
            QString word = match.captured(1);

            // Put word to hash
            QHash<QString, int>::iterator iter = hash.find(word);
            if (iter != hash.end()) {
                // Update word statistics
                iter.value()++;
            } else {
                // Add new word
                iter = hash.insert(word, 1);
            }

            // Check word in top
            int count = iter.value();
            if (count > minCount) {
                bool updated = false;
                // Find word in top
                int prevCount = count - 1;
                QMultiMap<int, QString>::iterator mapIter = top.find(prevCount);
                while (mapIter != top.end() && mapIter.key() == prevCount) {
                    if (word == mapIter.value()) {
                        // Update word count in top
                        top.erase(mapIter);
                        top.insert(count, word);
                        updated = true;
                        break;
                    }
                    mapIter++;
                }
                if (!updated) {
                    // Add word to top
                    top.insert(count, word);
                    if (top.size() > MaxTopItems)
                        top.erase(top.begin());
                }
                if (top.size() == MaxTopItems)
                    // Update minCount value
                    minCount = top.firstKey();
            }
        }

        processed++;
        if (processed - prevProcessed >= ProgressInterval || stream.atEnd()) {
        //if (timeout || stream.atEnd()) {
            QMap<QString, int> dict;
            QMultiMap<int, QString>::iterator topIter = top.begin();
            while (topIter != top.end()) {
                if (topIter.value() > 0)
                    dict.insert(topIter.value(), topIter.key());
                topIter++;
            }

            QStringList words;
            QStringList values;
            QMap<QString, int>::iterator dictIter = dict.begin();
            while (dictIter != dict.end()) {
                words << dictIter.key();
                values << QString::number(dictIter.value());
                dictIter++;
            }

            qint64 pos = stream.pos();
            int progress = 100 * pos / fileSize;

            emit progressChanged(words, values, progress);

            prevProcessed = processed;
            //timeout = false;
        }
    }
    file.close();
    emit textProcessed();
    qDebug() << "Process duration: " << QString::number(t.elapsed()) << " ms\n";
}
