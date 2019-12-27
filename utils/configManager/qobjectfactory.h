#ifndef QOBJECTFACTORY_H
#define QOBJECTFACTORY_H

#include <QByteArray>
#include <QMetaObject>
#include <QHash>
#include <QDebug>

class QObjectFactory
{
public:
    template<typename T>
    static void registerClass()
    {
        constructors().insert(QString(T::staticMetaObject.className()), &constructorHelper<T> );
    }

    static QObject* createObject(QString className, QObject* parent = nullptr )
    {
        Constructor constructor = constructors().value(className );
        if ( constructor == nullptr ) {
            qCritical() << QString("You should regist class before dynamic creation instance! Class nams: %1").arg(className);
            return nullptr;
        }
        QObject* newInstance = (*constructor)( parent );
        if(newInstance == nullptr){
            qCritical() << QString("Create new instance of class %1 failed!").arg(className);
        }
        return newInstance;
    }

private:
    typedef QObject* (*Constructor)( QObject* parent );

    template<typename T>
    static QObject* constructorHelper( QObject* parent )
    {
        return new T( parent );
    }

    static QHash<QString, Constructor>& constructors()
    {
        static QHash<QString, Constructor> instance;
        return instance;
    }
};



#endif // QOBJECTFACTORY_H
