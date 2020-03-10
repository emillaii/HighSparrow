#ifndef COMMONMETHOD_H
#define COMMONMETHOD_H

#include <QDateTime>
#include <QMetaEnum>
#include <QVariantList>
#include <QtCore>

#define SET_PRECISION(value, precision) (round(value * pow(10, precision)) / pow(10, precision))

QString combineString(const QString &seperator, const QList<QString> &stringList);

template <typename T>
QString combineNumber(const QString &seperator, const QList<T> &numberList)
{
    QString returnString;
    for (int i = 0; i < numberList.count(); i++)
    {
        returnString += QString::number(numberList[i]);
        if (i != numberList.count() - 1)
        {
            returnString += seperator;
        }
    }
    return returnString;
}

inline QString getCurrentDateTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}

template <typename T>
QVariantList toVariantList(QList<T> source)
{
    QVariantList qv;
    foreach (T item, source)
    {
        qv.append(item);
    }
    return qv;
}

int getMethodIndex(const QMetaObject *metaObj,
                                          QMetaMethod::MethodType methodType,
                                          QString methodName);

template <typename T>
class EnumHelper
{
public:
    EnumHelper(const QMetaObject &metaObj, QString enumTypeName)
        : className(metaObj.className()), enumTypeName(enumTypeName)
    {
        for (int i = 0; i < metaObj.enumeratorCount(); i++)
        {
            QMetaEnum qme = metaObj.enumerator(i);
            if (qme.name() == enumTypeName)
            {
                for (int j = 0; j < qme.keyCount(); j++)
                {
                    nameEnumMap[qme.key(j)] = static_cast<T>(qme.value(j));
                    enumNameMap[static_cast<T>(qme.value(j))] = qme.key(j);
                }
                return;
            }
        }
        qFatal("Did not find enum type '%s' in class: '%s'", enumTypeName.toUtf8().data(), metaObj.className());
    }

    QStringList names() const
    {
        return nameEnumMap.keys();
    }

    QString enumToName(T enumeration) const
    {
        return enumNameMap[enumeration];
    }

    T nameToEnum(QString name) const
    {
        if (!nameEnumMap.contains(name))
        {
            qFatal("Enum '%s' did not exist in enum type '%s' of class '%s'!", name.toUtf8().data(),
                   enumTypeName.toUtf8().data(), className.toUtf8().data());
        }
        return static_cast<T>(nameEnumMap[name]);
    }

private:
    QString className;
    QString enumTypeName;
    QMap<QString, T> nameEnumMap;
    QMap<T, QString> enumNameMap;
};

#define CHECK_POINTER(pointer)                                                                                         \
    if (pointer == nullptr)                                                                                            \
    {                                                                                                                  \
        qFatal("%s is null!", #pointer);                                                                               \
    }

#define SILICOOL_DELETE(pointer)                                                                                       \
    if (pointer != nullptr)                                                                                            \
    {                                                                                                                  \
        delete pointer;                                                                                                \
        pointer = nullptr;                                                                                             \
    }

#endif
