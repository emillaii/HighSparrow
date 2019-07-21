#ifndef FILECONTENT_H
#define FILECONTENT_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class FileContent : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString source
               READ source
               WRITE setSource
               NOTIFY sourceChanged)
    Q_PROPERTY(QString data
               READ data
               WRITE setData)
    explicit FileContent(QObject *parent = 0);

    Q_INVOKABLE QString read();
    Q_INVOKABLE bool write();

    QString source() { return mSource; };
    QString data() { return mData; };

public slots:
    void setSource(const QString& source) { mSource = source; };
    void setData(const QString& data) { mData = data; };
signals:
    void sourceChanged(const QString& source);
    void error(const QString& msg);

private:
    QString mSource;
    QString mData;
};

#endif // FILECONTENT_H
