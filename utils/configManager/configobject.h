#ifndef CONFIGOBJECT_H
#define CONFIGOBJECT_H

#include <QVariant>
#include <QMetaProperty>
#include <QDebug>
#include <QQuickItem>
#include <QJsonObject>
#include "../singletoninstances.h"
#include "../commonmethod.h"
#include "configbase.h"

class ConfigObject;

class SetObjectItemCommand: public CommandInterface
{
public:
    SetObjectItemCommand(ConfigObject* target, QString& propName, QVariant& oldValue, QVariant& newValue);

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigObject* target;
    QString propName;
    QVariant oldValue;
    QVariant newValue;
};


class ConfigObject: public ConfigBase
{
    Q_OBJECT

public:
    ConfigObject(QObject* parent=nullptr):ConfigBase(ConfigElementInfo::ConfigObj, parent)
    {

    }

    ///
    /// \brief setOptionalProperty This function should be called before calling init function
    /// \param configName   QT Property name
    /// \param options  The options will be the model of the corresponding ComboBox
    ///
    void setOptionalProperty(QString configName, QVariantList options);

    ///
    /// \brief setReadOnlyProperty  This function should be called before calling init function
    /// \param configName
    ///
    void setReadOnlyProperty(QString configName);

    ///
    /// \brief The hidden item will not be shown on UI.
    /// This function should be called before calling init function
    /// \param configName  QT Property name
    ///
    void hide(QString configName);


    ///
    /// \brief init Init must be called, otherwise the UI will not work.
    /// Init must be called after calling setOptionalProperty, setReadOnlyProperty and hide
    /// Calling this function in subclass`s constructor is recommended.
    /// If the subclass of ConfigObject has property which is pointer of ConfigObject or ConfigArray,
    /// make sure these pointers have been initialized! Otherwise the software may crashed!
    ///
    /// Warning: When you define an static instance of a class inheriting ConfigObject,
    /// you can not call this method in the constructor of that class! Otherwise software will crash.
    /// Because QT`s metaobject-system may not be initialized when constructing the static instance, i guess.
    ///
    void init();


    Q_INVOKABLE void connectNotifySignal(QString configName, QQuickItem* receiver, QString slotName)
    {
        auto prop = m_configProperties[configName];
        if(!prop.hasNotifySignal()){
            qCritical() << "Did not have notify signal! Config name: " << configName;
            return;
        }
        //        int slotIndex = receiver->metaObject()->indexOfSlot(slotName.toStdString().c_str());
        int slotIndex = -1;
        for(int i = 0; i < receiver->metaObject()->methodCount(); i++){
            auto method = receiver->metaObject()->method(i);
            if(method.methodType() == QMetaMethod::Slot &&
                    QString(method.name()) == slotName){
                slotIndex = method.methodIndex();
                break;
            }
        }
        if(slotIndex == -1){
            qCritical() << "Did not have slot! Slot name: " << slotName;
            return;
        }
        if(receiver->metaObject()->method(slotIndex).parameterCount() != 0){
            qWarning() << "Slot method having 0 paremeter is recommoned! Slot name: " << slotName;
        }
        QMetaObject::connect(this, prop.notifySignalIndex(), receiver, slotIndex);
    }

    Q_INVOKABLE QStringList getConfigNamesToShow() const
    {
        return m_configNamesToShow;
    }

    Q_INVOKABLE bool isReadOnly(QString configName) const
    {
        return m_readOnlyConfigNames.contains(configName);
    }

    Q_INVOKABLE bool isOptional(QString configName) const
    {
        return m_configOptions.contains(configName);
    }

    Q_INVOKABLE ConfigElementInfo::Type getConfigType(QString configName) const
    {
        return m_configTypes[configName];
    }

    Q_INVOKABLE QVariantList getOptions(QString configName) const
    {
        return m_configOptions[configName];
    }

    Q_INVOKABLE QVariant getConfig(QString configName) const
    {
#ifdef ConfigUnitTest
        qDebug() << QString("Get config, name: %1 ").arg(configName) << "Value: " << QObject::property(configName.toStdString().c_str());
#endif

        return QObject::property(configName.toStdString().c_str());
    }

    Q_INVOKABLE bool setConfig(QString configName, QVariant value)
    {
         QVariant oldValue = QObject::property(configName.toStdString().c_str());
         if(oldValue == value){
             return true;
         }
         return SI::cfgManager.executeCommand(new SetObjectItemCommand(this, configName, oldValue, value));
    }

    bool getIsInit() const
    {
        return isInit;
    }

    bool read(const QJsonValue& jsonValue) override;
    void write(QJsonValue& jsonValue) override;
    bool connectConfigChangedSignalToSlotImpl(QObject* receiver, int slotIndex, bool connect) override;

private:
    bool executeSetConfig(QString configName, QVariant value);

private:
    friend class ConfigFile;
    friend class SetObjectItemCommand;

    bool isInit = false;
    QStringList m_configNamesToShow;
    QStringList m_hiddenConfigNames;
    QStringList m_readOnlyConfigNames;
    QMap<QString, ConfigElementInfo::Type> m_configTypes;
    QMap<QString, QVariantList> m_configOptions;
    QMap<QString, QMetaProperty> m_configProperties;
};



#endif // CONFIGOBJECT_H
