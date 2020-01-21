#ifndef COMMONMETHOD_H
#define COMMONMETHOD_H

#include <QtCore>
#include <QDateTime>

class CommonMethod
{
public:
    static QString combineString(const QString& seperator, const QList<QString>& stringList);

    template<typename T>
    static QString combineNumber(const QString& seperator, const QList<T>& numberList);

    static QString getCurrentDateTime();
    static uint getIntFromHexOrDecString(QString value);
};

#endif
