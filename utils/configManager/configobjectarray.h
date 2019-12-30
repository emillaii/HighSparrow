#ifndef CONFIGOBJECTARRAY_H
#define CONFIGOBJECTARRAY_H

#include <QAbstractTableModel>
#include "configobject.h"
#include "qobjectfactory.h"
#include "../singletoninstances.h"
#include "../commonmethod.h"


class ConfigObjectArray;

class AddConfigObjectComand: public CommandInterface
{
public:
    AddConfigObjectComand(ConfigObjectArray* target, int index):
        target(target), index(index)
    {

    }

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigObjectArray* target;
    int index;
};

class RemoveConfigObjectCommand: public CommandInterface
{
public:
    RemoveConfigObjectCommand(ConfigObjectArray* target, int index, ConfigObject* oldConfigObj):
        target(target), index(index), oldConfigObj(oldConfigObj)
    {

    }

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigObjectArray* target;
    int index;
    ConfigObject* oldConfigObj;
};



class ConfigObjectArray: public ConfigBase
{
    Q_OBJECT

public:
    ///
    /// \brief ConfigObjectArray
    /// \param configObjClassName This class must inherit ConfibObject, and the parameter of it`s constructor must be (QObject* parent)
    /// \param parent
    ///
    ConfigObjectArray(QString configObjClassName, QObject* parent=nullptr);

    ConfigObject* const &at(int index) const
    {
        if(index < 0 || index >= this->count()){
            qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(this->count());
        }
        return configObjects.at(index);
    }
    ConfigObject* const &operator[](int index) const
    {
        return this->at(index);
    }
    ConfigObject* &operator[](int index)
    {
        if(index < 0 || index >= this->count()){
            qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(this->count());
        }
        return configObjects[index];
    }
    int count() const
    {
        return configObjects.count();
    }

    Q_INVOKABLE bool add(int index)
    {
        return SI::cfgManager.executeCommand(new AddConfigObjectComand(this, index));
    }

    Q_INVOKABLE bool remove(int index)
    {
        if(index < 0 || index >= this->count()){
            qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(this->count());
            return false;
        }
        ConfigObject* oldConfigObj = configObjects[index];
        return SI::cfgManager.executeCommand(new RemoveConfigObjectCommand(this, index, oldConfigObj));
    }

    Q_INVOKABLE int configCount() const
    {
        return count();
    }

    Q_INVOKABLE QStringList getTitles() const
    {
        return templateObject->getConfigNamesToShow();
    }

    Q_INVOKABLE QObject* getConfig(int index) const
    {
        qDebug() << this->at(index);
        return this->at(index);
    }

    // ConfigBase interface
public:
    virtual bool read(const QJsonValue &jsonValue) override;
    virtual void write(QJsonValue &jsonValue) override;
    bool connectConfigChangedSignalToSlotImpl(QObject* receiver, int slotIndex, bool connect) override;

signals:
    void configInserted(int index, ConfigObject* value);
    void configRemoved(int index);

private:
    bool executeAddConfigObject(int index, ConfigObject* instance=nullptr);
    bool executeRemoveConfigObject(int index);

private:
    friend class AddConfigObjectComand;
    friend class RemoveConfigObjectCommand;

    QString configObjClassName;
    QList<ConfigObject*> configObjects;
    ConfigObject* templateObject;

    QObject* configChangedSignalReceiver = nullptr;
    int configChangedReceiverSlotIndex;
};

#endif // CONFIGOBJECTARRAY_H
