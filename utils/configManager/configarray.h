#ifndef CONFIGARRAY_H
#define CONFIGARRAY_H

#include "configbase.h"
#include "../singletoninstances.h"
#include "../commonmethod.h"

class ConfigArray;

class AddArrayItemCommand: public CommandInterface
{
public:
    AddArrayItemCommand(ConfigArray* target, int index);

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigArray* target;
    int index;
};

class RemoveArrayItemCommand: public CommandInterface
{
public:
    RemoveArrayItemCommand(ConfigArray* target, int index, QVariant& oldValue);

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigArray *target;
    int index;
    QVariant oldValue;
};

class SetArrayItemCommand: public CommandInterface
{
public:
    SetArrayItemCommand(ConfigArray* target, int index, QVariant& oldValue, QVariant& newValue);

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

    ConfigArray* target;
    int index;
    QVariant oldValue;
    QVariant newValue;
};


///
/// \brief The ConfigArray class
/// If you want to change this list(add, remove or change), you have to call the corresponding method of ConfigArray,
/// otherwise these changes would not be auto saved!
///
class ConfigArray: public ConfigBase, public QVariantList
{
    Q_OBJECT

public:
    ConfigArray(ConfigElementInfo::Type elementType, QObject* parent=nullptr):
        ConfigBase (ConfigElementInfo::ConfigArr, parent), elementType(elementType)
    {
        if(elementType == ConfigElementInfo::ConfigArr){
            qFatal("The element of ConfigArray can not be another ConfigArray!");
        }
        if(elementType == ConfigElementInfo::ConfigObj){
            qFatal("The element of ConfigArray can not be ConfigObj! You can use ConfigObjectArray instead.");
        }
    }

    void setOptionalProperty(QVariantList& options);

    void setReadOnlyProperty();


    Q_INVOKABLE int configCount()
    {
        return this->count();
    }

    Q_INVOKABLE bool add(int index)
    {
        return SI::cfgManager.executeCommand(new AddArrayItemCommand(this, index));
    }

    Q_INVOKABLE bool remove(int index)
    {
        if(index < 0 || index >= this->count()){
            qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(this->count());
            return false;
        }
        QVariant oldValue = this->at(index);
        return SI::cfgManager.executeCommand(new RemoveArrayItemCommand(this, index, oldValue));
    }

    Q_INVOKABLE bool isReadOnly() const
    {
        return m_isReadOnly;
    }

    Q_INVOKABLE bool isOptional() const
    {
        return m_isOptional;
    }

    Q_INVOKABLE ConfigElementInfo::Type getConfigType() const
    {
        return elementType;
    }

    Q_INVOKABLE QVariantList getOptions(QString name) const
    {
        Q_UNUSED(name)
        return m_options;
    }

    Q_INVOKABLE QVariant getConfig(int index) const
    {
        if(index < 0 || index >= this->count()){
            qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(this->count());
            return QVariant();
        }

#ifdef ConfigUnitTest
        qDebug() << QString("Get config, index: %1 ").arg(index) << "Value: " << this->at(index);
#endif

        return this->at(index);
    }

    Q_INVOKABLE bool setConfig(int index, QVariant value)
    {
        if(index < 0 || index >= this->count()){
            qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(this->count());
            return false;
        }

        if(elementType == ConfigElementInfo::Int)
        {
            if(value.canConvert<int>()){
                value = value.toInt();
            }else {
                qCritical() << QString("Can not convert to type int. value: ") << value;
                return false;
            }
        }

        if(elementType == ConfigElementInfo::Double)
        {
            if(value.canConvert<double>()){
                value = value.toDouble();
            }else {
                qCritical() << QString("Can not convert to type double. value: ") << value;
                return false;
            }
        }

        if(elementType == ConfigElementInfo::Bool)
        {
            if(value.canConvert<bool>()){
                value = value.toBool();
            }else {
                qCritical() << QString("Can not convert to type bool. value: ") << value;
                return false;
            }
        }

        QVariant oldValue = this->at(index);
        if(oldValue == value){
            return true;
        }
        return SI::cfgManager.executeCommand(new SetArrayItemCommand(this, index, oldValue, value));
    }

    // ConfigBase interface
public:
    virtual bool connectConfigChangedSignalToSlotImpl(QObject *receiver, int slotIndex, bool connect) override;
    virtual bool read(const QJsonValue &jsonValue) override;
    virtual void write(QJsonValue &jsonValue) override;

signals:
    void configInserted(int index, QVariant value);
    void configRemoved(int index);
    void configChanged(int index, QVariant value);

private:
    bool setSimpleTypeConfig(int index, QVariant& value)
    {
        if(index < 0 || index >= this->count()){
            qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(this->count());
            return false;
        }

        auto ValueType = ConfigElementInfo::getType(value.type());
        if(ValueType == ConfigElementInfo::Double && elementType == ConfigElementInfo::Int){
            value = value.toInt();
        }else if(ValueType == ConfigElementInfo::Int && elementType == ConfigElementInfo::Double){
            value = value.toDouble();
        }else {
            if(ValueType != elementType){
                qCritical() << QString("Set config failed! Index: %1, Value type: %2, target type: %3").
                               arg(index).arg(ValueType).arg(elementType);
                return false;
            }
        }
        replace(index, value);
        return true;
    }

    bool executeSetConfig(int index, QVariant value);

    bool executeAddItem(int index);

    bool executeRemoveItem(int index);

private:
    friend class ConfigFile;
    friend class ConfigObject;
    friend class SetArrayItemCommand;
    friend class AddArrayItemCommand;
    friend class RemoveArrayItemCommand;

    ConfigElementInfo::Type elementType;
    bool m_isOptional = false;
    bool m_isReadOnly = false;
    QVariantList m_options;
};

#endif // CONFIGARRAY_H
