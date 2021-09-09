#include "textthread.h"

#include <QFile>
#include <QTextStream>
#include <QElapsedTimer>
#include <QRegularExpression>

TextThread::TextThread(QObject *parent) : QObject(parent)
{
}

void TextThread::run()
{
    const int ProgressInterval = 1000;
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

    QFile inFile("in.txt");
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream inStream(&inFile);

    QFile outFile("out.txt");
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream outStream(&outFile);

    qint64 fileSize = inFile.size();
    qint64 prevProcessed = 0;
    qint64 processed = 0;

    QElapsedTimer timer;
    timer.start();
    while (!inStream.atEnd()) {

        QString str;
        inStream >> str;

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
        if (processed - prevProcessed >= ProgressInterval || inStream.atEnd()) {
            QMap<QString, int> dict;
            QMultiMap<int, QString>::iterator topIter = top.begin();
            while (topIter != top.end()) {
                if (topIter.value() > 0)
                    dict.insert(topIter.value(), topIter.key());
                topIter++;
            }

            QString result;
            QMap<QString, int>::iterator dictIter = dict.begin();
            while (dictIter != dict.end()) {
                result += dictIter.key() + ":" + QString::number(dictIter.value()) + " ";
                dictIter++;
            }

            qint64 pos = inStream.pos();
            int progress = 100 * pos / fileSize;
            //QString info = QString::number(progress) + " " + QString::number(fileSize) + " " + QString::number(pos) + " " + QString::number(processed);
            QString info = QString::number(progress) + " " + QString::number(pos) + " ";

            outStream << info << "  " << result << "\n";
            prevProcessed = processed;
        }
    }
    outStream << "Process duration: " << QString::number(timer.elapsed()) << " ms\n";
    outFile.close();
    inFile.close();
}
