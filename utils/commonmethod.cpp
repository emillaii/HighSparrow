#include "commonmethod.h"


QString CommonMethod::combineString(const QString &seperator, const QList<QString> &stringList)
{
    QString returnString;
    for(int i = 0; i < stringList.count(); i++)
    {
        returnString += stringList[i];
        if(i != stringList.count() - 1){
            returnString += seperator;
        }
    }
    return returnString;
}

QString CommonMethod::getCurrentDateTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}

uint CommonMethod::getIntFromHexOrDecString(QString value)
{
    bool ok = false;
    uint result = 0;
    result = value.toInt(&ok, 16);       // hex == 255, ok == true
    if (ok) {
        return result;
    } else {
        result = value.toInt(&ok, 10);       // dec == 0, ok == false
    }
    return result;
}

template<typename T>
QString CommonMethod::combineNumber(const QString &seperator, const QList<T> &numberList)
{
    QString returnString;
    for(int i = 0; i < numberList.count(); i++)
    {
        returnString += QString::number(numberList[i]);
        if(i != numberList.count() - 1){
            returnString += seperator;
        }
    }
    return returnString;
}
