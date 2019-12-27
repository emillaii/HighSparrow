#include "rollbackfile.h"


RollbackFile::RollbackFile()
{

}

void RollbackFile::init(QString folder, QString fileName, qint64 maxSize, int nBackupFile, bool flushImmediately)
{
    this->folder = folder;
    this->fileName = fileName;
    this->maxSize = maxSize;
    this->nBackupFile = nBackupFile;
    this->flushImmediately = flushImmediately;
    dateForLogFolderName = QDateTime::currentDateTime();
    initFileStream();
    stream.setDevice(&file);
    stream.setCodec(QTextCodec::codecForName("utf8"));
}

void RollbackFile::appendLine(QString s)
{
    QMutexLocker tmpLocker(&locker);

    if(QDateTime::currentDateTime().date() != dateForLogFolderName.date())
    {
        dateChanged();
    }
    s.append("\n");
    QByteArray bytes = s.toUtf8();
    if(currentSize + bytes.length() > maxSize)
    {
        rollBack();
    }
    stream << bytes;
    if(flushImmediately)
    {
        stream.flush();
    }
    currentSize += bytes.length();
}

RollbackFile::~RollbackFile()
{
    stream.flush();
    file.close();
}

void RollbackFile::initFileStream()
{
    QString fullPath = folder + "/" + dateForLogFolderName.toString("yyyy-MM-dd");
    QString fullName = fullPath + "/" + fileName + ".log";
    QDir dir(fullPath);
    file.setFileName(fullName);
    if(file.exists()){
        QFileInfo fileInfo(file);
        currentSize = fileInfo.size();
    }
    else {
        currentSize = 0;
        if(!dir.exists()){
            dir.mkpath(dir.absolutePath());
        }
    }
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

void RollbackFile::rollBack()
{
    stream.flush();
    file.close();

    QString fullPath = folder + "/" + dateForLogFolderName.toString("yyyy-MM-dd");
    QDir dir(fullPath);
    QStringList filter;
    filter.append(fileName + ".log");
    filter.append(fileName + ".log.[0-9]*");
    QFileInfoList fileInfos = dir.entryInfoList(filter);
    QList<int> postfixes;
    foreach(QFileInfo fileInfo, fileInfos)
    {
        QStringList tmpList = fileInfo.fileName().split('.');
        if(tmpList.count() > 0)
        {
            QString postFix = tmpList.last();
            if(postFix == "log")
            {
                postfixes.append(0);
            }
            else {
                postfixes.append(postFix.toInt());
            }
        }
    }
    qSort(postfixes.begin(), postfixes.end());
    QString tmpName = fullPath + "/" + fileName + ".log";
    if(postfixes.count() >= nBackupFile && postfixes.count() > 0)
    {
        if(nBackupFile == 1)
        {
            QFile::remove(tmpName);
        }
        else {
            QFile::remove(tmpName + "." + QString::number(postfixes.last()));
        }
        postfixes.removeLast();
    }
    for (int i = postfixes.count() - 1; i >= 0; i--) {
        int postfix = postfixes[i];
        if(postfix == 0)
        {
            QFile::rename(tmpName, tmpName + ".1");
        }
        else {
            QFile::rename(tmpName + "." + QString::number(postfix),
                          tmpName + "." + QString::number(postfix + 1));
        }
    }

    initFileStream();
}

void RollbackFile::dateChanged()
{
    stream.flush();
    file.close();
    dateForLogFolderName = QDateTime::currentDateTime();
    initFileStream();
}
