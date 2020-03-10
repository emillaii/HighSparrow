#include "commonmethod.h"

QString combineString(const QString &seperator, const QList<QString> &stringList)
{
    QString returnString;
    for (int i = 0; i < stringList.count(); i++)
    {
        returnString += stringList[i];
        if (i != stringList.count() - 1)
        {
            returnString += seperator;
        }
    }
    return returnString;
}

int getMethodIndex(const QMetaObject *metaObj,
                                          QMetaMethod::MethodType methodType,
                                          QString methodName)
{
    for (int i = 0; i < metaObj->methodCount(); i++)
    {
        QMetaMethod metaMethod = metaObj->method(i);
        if (metaMethod.methodType() == methodType && metaMethod.name() == methodName)
        {
            return i;
        }
    }
    return -1;
}
