#include "filecontent.h"
#include <QDebug>
#include "qurl.h"

FileContent::FileContent(QObject *parent) :
    QObject(parent)
{

}

QString FileContent::read()
{
    qDebug(mSource.toStdString().c_str());
    if (mSource.isEmpty()){
        emit error("source is empty");
        return QString();
    }
    QUrl url(mSource);
    qDebug(url.path().toStdString().c_str());
    QFile file(url.path().remove(0,1));
    QString fileContent;
    if ( file.open(QIODevice::ReadOnly) ) {
        QString line;
        QTextStream t( &file );
        do {
            line = t.readLine();
            fileContent += line;
         } while (!line.isNull());

        file.close();
    } else {
        emit error("Unable to open the file");
        return QString();
    }
    return fileContent;
}

bool FileContent::write()
{
    if (mSource.isEmpty())
        return false;

    QUrl url(mSource);
    qDebug(url.path().toStdString().c_str());
    QFile file(url.path().remove(0,1));
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return false;

    QTextStream out(&file);
    out << mData;

    file.close();

    return true;
}
